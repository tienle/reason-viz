open ReasonViz__Types;

module Canvas = ReasonViz__Canvas;
module RN = ReasonViz__Node;
module RE = ReasonViz__Edge;

type t = {
  canvas: Canvas.t,
  group: Canvas.Group.t,
  nodesMap: Js.Dict.t(RN.t),
  edgesMap: Js.Dict.t(RE.t),
  mutable nodes: list(RN.t),
  mutable edges: list(RE.t),
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
  };
};

let addNode = (graph, model) => {
  let node = RN.make(~model, ~parentGroup=graph.group);
  Js.Dict.set(graph.nodesMap, model.id, node);

  module Shape = (val ReasonViz__NodeShape.get(model.shape));
  node.shape = Shape.draw(node);
  node;
};

let addNodes = (g, nodes) => {
  g.nodes = List.map(addNode(g), nodes);
};

let addEdge = (graph, model) => {
  let edge =
    RE.make(~nodesMap=graph.nodesMap, ~model, ~parentGroup=graph.group);
  Js.Dict.set(graph.edgesMap, model.id, edge);

  module Shape = (val ReasonViz__EdgeShape.get(model.shape));
  Shape.draw(edge);
  edge;
};

let addEdges = (g, edges) => {
  g.edges = List.map(addEdge(g), edges);
};

let paint = g => {
  // TODO: emit beforepaint
  // TODO: emit afterpaint
  Canvas.draw(g.canvas);
};

let render = g => {
  paint(g);
};
