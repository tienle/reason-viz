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

let create: (~graphOptions: ReasonViz__Graph_Options.t) => t;
let addNodes: (t, list(ReasonViz__Node.Model.t)) => unit;
let addEdges: (t, list(ReasonViz__Edge.Model.t)) => unit;
let render: t => unit;


