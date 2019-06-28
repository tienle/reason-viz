open ReasonViz__Types;

module Canvas = ReasonViz__Canvas;
module RN = ReasonViz__Node;
module RE = ReasonViz__Edge;
module Event = ReasonViz__Event;
module Util = ReasonViz__Utils;
type event = {
  .
  "x": int,
  "y": int,
  "canvasX": int,
  "canvasY": int,
  "clientX": int,
  "clientY": int,
  "timeStamp": int,
  "type": string,
};

type canvasEvent = (event, Canvas.t);
type nodeEvent = (event, RN.t);
type edgeEvent = (event, RE.t);

type t = {
  canvas: Canvas.t,
  group: Canvas.Group.t,
  nodeGroup: Canvas.Group.t,
  edgeGroup: Canvas.Group.t,
  nodesMap: Js.Dict.t(RN.t),
  edgesMap: Js.Dict.t(RE.t),
  mutable nodes: list(RN.t),
  mutable edges: list(RE.t),
  mutable cleanableEffects: list(unit => unit),
  mutable dragging: bool,
  events,
}
and events = {
  onBeforePaint: Event.t(t),
  onAfterPaint: Event.t(t),
  onBeforeAddEdge: Event.t(RE.t),
  onAfterAddEdge: Event.t(RE.t),
  onBeforeAddNode: Event.t(RN.t),
  onAfterAddNode: Event.t(RN.t),
  onCanvasClick: Event.t(canvasEvent),
  onCanvasMouseDown: Event.t(canvasEvent),
  onCanvasMouseUp: Event.t(canvasEvent),
  onCanvasDblClick: Event.t(canvasEvent),
  onCanvasContextMenu: Event.t(canvasEvent),
  onCanvasMouseEnter: Event.t(canvasEvent),
  onCanvasMouseOut: Event.t(canvasEvent),
  onCanvasMouseOver: Event.t(canvasEvent),
  onCanvasMouseMove: Event.t(canvasEvent),
  onCanvasMouseLeave: Event.t(canvasEvent),
  onCanvasDragStart: Event.t(canvasEvent),
  onCanvasDragEnd: Event.t(canvasEvent),
  onCanvasDrag: Event.t(canvasEvent),
  onCanvasDragEnter: Event.t(canvasEvent),
  onCanvasDragLeave: Event.t(canvasEvent),
  onCanvasDrop: Event.t(canvasEvent),
  onNodeClick: Event.t(nodeEvent),
  onNodeMouseDown: Event.t(nodeEvent),
  onNodeMouseUp: Event.t(nodeEvent),
  onNodeDblClick: Event.t(nodeEvent),
  onNodeContextMenu: Event.t(nodeEvent),
  onNodeMouseEnter: Event.t(nodeEvent),
  onNodeMouseOut: Event.t(nodeEvent),
  onNodeMouseOver: Event.t(nodeEvent),
  onNodeMouseMove: Event.t(nodeEvent),
  onNodeMouseLeave: Event.t(nodeEvent),
  onNodeDragStart: Event.t(nodeEvent),
  onNodeDragEnd: Event.t(nodeEvent),
  onNodeDrag: Event.t(nodeEvent),
  onNodeDragEnter: Event.t(nodeEvent),
  onNodeDragLeave: Event.t(nodeEvent),
  onNodeDrop: Event.t(nodeEvent),
  onEdgeClick: Event.t(edgeEvent),
  onEdgeMouseDown: Event.t(edgeEvent),
  onEdgeMouseUp: Event.t(edgeEvent),
  onEdgeDblClick: Event.t(edgeEvent),
  onEdgeContextMenu: Event.t(edgeEvent),
  onEdgeMouseEnter: Event.t(edgeEvent),
  onEdgeMouseOut: Event.t(edgeEvent),
  onEdgeMouseOver: Event.t(edgeEvent),
  onEdgeMouseMove: Event.t(edgeEvent),
  onEdgeMouseLeave: Event.t(edgeEvent),
  onEdgeDragStart: Event.t(edgeEvent),
  onEdgeDragEnd: Event.t(edgeEvent),
  onEdgeDrag: Event.t(edgeEvent),
  onEdgeDragEnter: Event.t(edgeEvent),
  onEdgeDragLeave: Event.t(edgeEvent),
  onEdgeDrop: Event.t(edgeEvent),
};
