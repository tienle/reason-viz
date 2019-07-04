open ReasonViz__Types;

module Canvas = ReasonViz__Canvas;
module Event = ReasonViz__Event;
module Util = ReasonViz__Utils;

module ShapeValue = {
  exception Invalid;

  type t =
    | Int(int)
    | PairInt(int, int)
    | Bool(bool)
    | Float(float)
    | Str(string);

  let toInt =
    fun
    | Int(v) => v
    | _ => raise(Invalid);

  let toFloat =
    fun
    | Float(v) => v
    | _ => raise(Invalid);

  let toPairInt =
    fun
    | PairInt(v1, v2) => (v1, v2)
    | _ => raise(Invalid);

  let toBool =
    fun
    | Bool(v) => v
    | _ => raise(Invalid);

  let toString =
    fun
    | Str(v) => v
    | _ => raise(Invalid);

  let getExn = Belt.Option.getExn;
};

module PropsList = {
  type props = list((string, ShapeValue.t));

  type t = {
    get: string => option(ShapeValue.t),
    getExn: string => ShapeValue.t,
    props,
  };

  let make = props => {
    let dict = props |> Js.Dict.fromList;

    {
      get: prop => Js.Dict.get(dict, prop),
      getExn: prop => Js.Dict.unsafeGet(dict, prop),
      props,
    };
  };
};

module EdgeLabel = {
  [@bs.deriving jsConverter]
  type position = [ | `start | `center | `middle | [@bs.as "end"] `end_];

  type cfg = {
    position,
    autoRotate: bool,
    refX: int,
    refY: int,
    styles: StylesList.t,
  };

  type t = {
    text: string,
    cfg: option(cfg),
  };
};

module NodeLabel = {
  type position = [ | `top | `bottom | `left | `right | `center];

  type cfg = {
    position,
    offset: int,
    styles: StylesList.t,
  };

  type t = {
    text: string,
    cfg: option(cfg),
  };
};

module NodeModel = {
  type t = {
    id: string,
    props: PropsList.t,
    styles: StylesList.t,
    anchorPoints: list((float, float)),
    label: option(NodeLabel.t),
    shape: string,
    size,
  };
};

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

type t = {
  canvas: Canvas.t,
  group: Canvas.Group.t,
  nodeGroup: Canvas.Group.t,
  edgeGroup: Canvas.Group.t,
  nodesMap: Js.Dict.t(node),
  edgesMap: Js.Dict.t(edge),
  mutable nodes: list(node),
  mutable edges: list(edge),
  mutable cleanableEffects: list(unit => unit),
  mutable dragging: bool,
  events,
}
and node = {
  graph: t,
  model: NodeModel.t,
  group: Canvas.Group.t,
  mutable shape: Canvas.Shape.t,
  mutable x: float,
  mutable y: float,
  state: Js.Dict.t(string),
  mutable edges: list(edge),
}
and edge = {
  graph: t,
  model: edgeModel,
  group: Canvas.Group.t,
  mutable shape: Canvas.Shape.t,
  state: Js.Dict.t(string),
}
and vertex = [ | `Point(point) | `NodeId(string) | `Node(node)]
and edgeModel = {
  id: string,
  mutable source: vertex,
  mutable target: vertex,
  mutable sourcePoint: point,
  mutable targetPoint: point,
  shape: string,
  styles: StylesList.t,
  label: option(EdgeLabel.t),
  sourceAnchor: string,
  targetAnchor: string,
  controlPoints: array(point),
  linkCenter: bool,
  size: int,
}
and events = {
  onBeforePaint: Event.t(t),
  onAfterPaint: Event.t(t),
  onBeforeAddEdge: Event.t(edge),
  onAfterAddEdge: Event.t(edge),
  onBeforeAddNode: Event.t(node),
  onAfterAddNode: Event.t(node),
  onNodeStateUpdated: Event.t((node, string, string, string)),
  onEdgeStateUpdated: Event.t((edge, string, string, string)),
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
  onClick: Event.t(generalEvent),
  onMouseDown: Event.t(generalEvent),
  onMouseUp: Event.t(generalEvent),
  onDblClick: Event.t(generalEvent),
  onContextMenu: Event.t(generalEvent),
  onMouseEnter: Event.t(generalEvent),
  onMouseOut: Event.t(generalEvent),
  onMouseOver: Event.t(generalEvent),
  onMouseMove: Event.t(generalEvent),
  onMouseLeave: Event.t(generalEvent),
  onDragStart: Event.t(generalEvent),
  onDragEnd: Event.t(generalEvent),
  onDrag: Event.t(generalEvent),
  onDragEnter: Event.t(generalEvent),
  onDragLeave: Event.t(generalEvent),
  onDrop: Event.t(generalEvent),
}
and canvasEvent = (event, Canvas.t)
and nodeEvent = (event, node)
and edgeEvent = (event, edge)
and generalEvent = (event, option(generalItem))
and generalItem = [ | `Edge(edge) | `Node(node)];

