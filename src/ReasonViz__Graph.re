open ReasonViz__Types;

module Canvas = ReasonViz__Canvas;
module RN = ReasonViz__Node;
module RE = ReasonViz__Edge;
module Event = ReasonViz__Event;

type t = {
  canvas: Canvas.t,
  group: Canvas.Group.t,
  nodesMap: Js.Dict.t(RN.t),
  edgesMap: Js.Dict.t(RE.t),
  mutable nodes: list(RN.t),
  mutable edges: list(RE.t),
  onBeforePaint: Event.t(t),
  onAfterPaint: Event.t(t),
  onBeforeAddEdge: Event.t(RE.t),
  onAfterAddEdge: Event.t(RE.t),
  onBeforeAddNode: Event.t(RN.t),
  onAfterAddNode: Event.t(RN.t),
};

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
    Canvas.addGroup(canvas, {"id": id, "className": "root-container"});

  {
    canvas,
    group,
    nodesMap: Js.Dict.empty(),
    edgesMap: Js.Dict.empty(),
    nodes: [],
    edges: [],
    onBeforePaint: Event.create(),
    onAfterPaint: Event.create(),
    onBeforeAddEdge: Event.create(),
    onAfterAddEdge: Event.create(),
    onBeforeAddNode: Event.create(),
    onAfterAddNode: Event.create(),
  };
};

let addNode = (graph, model) => {
  let node = RN.make(~model, ~parentGroup=graph.group);
  Js.Dict.set(graph.nodesMap, model.id, node);

  Event.dispatch(graph.onBeforeAddNode, node);
  module Shape = (val ReasonViz__NodeShape.get(model.shape));
  Shape.draw(node);
  Event.dispatch(graph.onAfterAddNode, node);
  node;
};

let addNodes = (g, nodes) => {
  g.nodes = List.map(addNode(g), nodes);
};

let addEdge = (graph, model) => {
  let edge =
    RE.make(~nodesMap=graph.nodesMap, ~model, ~parentGroup=graph.group);
  Js.Dict.set(graph.edgesMap, model.id, edge);

  Event.dispatch(graph.onBeforeAddEdge, edge);
  module Shape = (val ReasonViz__EdgeShape.get(model.shape));
  Shape.draw(edge);
  Event.dispatch(graph.onAfterAddEdge, edge);
  edge;
};

let addEdges = (g, edges) => {
  g.edges = List.map(addEdge(g), edges);
};

let paint = g => {
  Event.dispatch(g.onBeforePaint, g);
  Canvas.draw(g.canvas);
  Event.dispatch(g.onAfterPaint, g);
};

let render = g => {
  paint(g);
};
