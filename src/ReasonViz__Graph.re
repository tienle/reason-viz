module Canvas = ReasonViz__Canvas;
module RN = ReasonViz__Node;
module RE = ReasonViz__Edge;
module Event = ReasonViz__Event;
module EventHandler = ReasonViz__EventHandler;
module Util = ReasonViz__Utils;

type t = ReasonViz__GraphTypes.t;

let create = (~graphOptions) => {
  let canvas =
    Canvas.create(
      ~containerId=graphOptions.ReasonViz__Graph_Options.containerId,
      ~width=graphOptions.width,
      ~height=graphOptions.height,
      ~renderer=graphOptions.renderer,
      ~pixelRatio=?graphOptions.pixelRatio,
      (),
    );
  let id: string = [%raw "canvas.get('el').id"];
  let group =
    Canvas.addGroup(
      canvas,
      {"id": id ++ "-root", "className": "root-container"},
    );
  let edgeGroup =
    Canvas.addGroup(
      canvas,
      {"id": id ++ "-edge", "className": "edge-container", "zIndex": 1},
    );
  let nodeGroup =
    Canvas.addGroup(
      canvas,
      {"id": id ++ "-node", "className": "node-container", "zIndex": 10},
    );

  let g: t = {
    canvas,
    group,
    nodeGroup,
    edgeGroup,
    nodesMap: Js.Dict.empty(),
    edgesMap: Js.Dict.empty(),
    nodes: [],
    edges: [],
    cleanableEffects: [],
    dragging: false,
    events: EventHandler.createEmptyEvents(),
  };

  EventHandler.bindEvents(g);

  g;
};

let addNode = (graph: t, model) => {
  let node = RN.make(~model, ~parentGroup=graph.nodeGroup);
  Js.Dict.set(graph.nodesMap, model.id, node);

  Event.dispatch(graph.events.onBeforeAddNode, node);
  module Shape = (val ReasonViz__NodeShape.get(model.shape));
  Shape.draw(node);
  Event.dispatch(graph.events.onAfterAddNode, node);
  node;
};

let addNodes = (g: t, nodes) => {
  g.nodes = List.map(addNode(g), nodes);
};

let addEdge = (graph: t, model) => {
  let edge =
    RE.make(~nodesMap=graph.nodesMap, ~model, ~parentGroup=graph.edgeGroup);
  Js.Dict.set(graph.edgesMap, model.id, edge);

  let addEdgeToNode = (node: RE.Model.vertex, edge) => {
    switch (node) {
    | `Node(node) => node.edges = node.edges @ [edge]
    | _ => ()
    };
  };

  addEdgeToNode(edge.model.source, edge);
  addEdgeToNode(edge.model.target, edge);

  Event.dispatch(graph.events.onBeforeAddEdge, edge);
  module Shape = (val ReasonViz__EdgeShape.get(model.shape));
  Shape.draw(edge);
  Event.dispatch(graph.events.onAfterAddEdge, edge);
  edge;
};

let addEdges = (g: t, edges) => {
  g.edges = List.map(addEdge(g), edges);
};

let setState = (g: t, item, key, value) => {
  switch (item) {
  | `Node(node) => RN.setState(node, key, value)
  | `Edge(edge) => RE.setState(edge, key, value)
  };
  Event.dispatch(g.events.onStateUpdate, (item, key, value));
};

let paint = (g: t) => {
  Event.dispatch(g.events.onBeforePaint, g);
  Canvas.draw(g.canvas);
  Event.dispatch(g.events.onAfterPaint, g);
};

let render = (g: t) => {
  paint(g);
};

let destroy = g => {
  // TODO: clean effects and allocations
  EventHandler.clean(g);
};
