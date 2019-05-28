open Belt;
open ReasonViz__Types;

module Canvas = ReasonViz__Canvas;

type t('a) = {
  canvas: Canvas.t,
  group: Canvas.Group.t,
  mutable itemMap: StringHash.t(ReasonViz__Node_Type.t('a)),
  mutable nodes: list(ReasonViz__Node_Type.t('a)),
  /* mutable edges: list(edge), */
};

let create = (~graphOptions: ReasonViz__Graph_Options.t) => {
  let canvas =
    Canvas.create(
      ~containerId=graphOptions.containerId,
      ~width=graphOptions.width,
      ~height=graphOptions.height,
      ~renderer=graphOptions.renderer,
      ~pixelRatio=?graphOptions.pixelRatio,
      (),
    );
  let id: string = [%raw "canvas.get('el').id"];
  let group =
    Canvas.addGroup(canvas, {"id": id, "className": "root-container"});
  {canvas, group, itemMap: StringHash.create(10), nodes: []};
};

let addNode = (g, model: ReasonViz__Data.node('a)) => {
  // TODO: nodeStyle: default selected active
  let node = ReasonViz__Node.create(~model, ~parentGroup=g.group, ());
  g.nodes = g.nodes @ [node];
  StringHash.add(g.itemMap, node.id, node);
  // emit after add item
  // autopaint
  node;
};

let addEdge = (g, edge) => ();

// TODO: why type not being infered from .rei
let parse = (g: t('a), ~data: ReasonViz__Data.t('a)) => {
  List.forEach(data.nodes, n => addNode(g, n));
  g;
};

let paint = g => {
  // TODO: emit beforepaint
  // TODO: emit afterpaint
  Canvas.draw(g.canvas);
};

let render = g => {
  Canvas.clear(g.canvas);
  paint(g);
};
