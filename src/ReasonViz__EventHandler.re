open ReasonViz__Types;

module Canvas = ReasonViz__Canvas;
module Event = ReasonViz__Event;
module Util = ReasonViz__Utils;

type t = ReasonViz__GraphTypes.t;

module View = {
  let getPointByCanvas = (g: t, point) => {
    let viewportMatrix = Canvas.Group.getMatrix(g.group);
    Util.Math.invertMatrix(point, viewportMatrix);
  };
};

let createEmptyEvents = (): ReasonViz__GraphTypes.events => {
  onBeforePaint: Event.create(),
  onAfterPaint: Event.create(),
  onBeforeAddEdge: Event.create(),
  onAfterAddEdge: Event.create(),
  onBeforeAddNode: Event.create(),
  onAfterAddNode: Event.create(),
  onNodeStateUpdated: Event.create(),
  onEdgeStateUpdated: Event.create(),
  onCanvasClick: Event.create(),
  onCanvasMouseDown: Event.create(),
  onCanvasMouseUp: Event.create(),
  onCanvasDblClick: Event.create(),
  onCanvasContextMenu: Event.create(),
  onCanvasMouseEnter: Event.create(),
  onCanvasMouseOut: Event.create(),
  onCanvasMouseOver: Event.create(),
  onCanvasMouseMove: Event.create(),
  onCanvasMouseLeave: Event.create(),
  onCanvasDragStart: Event.create(),
  onCanvasDragEnd: Event.create(),
  onCanvasDrag: Event.create(),
  onCanvasDragEnter: Event.create(),
  onCanvasDragLeave: Event.create(),
  onCanvasDrop: Event.create(),
  onNodeClick: Event.create(),
  onNodeMouseDown: Event.create(),
  onNodeMouseUp: Event.create(),
  onNodeDblClick: Event.create(),
  onNodeContextMenu: Event.create(),
  onNodeMouseEnter: Event.create(),
  onNodeMouseOut: Event.create(),
  onNodeMouseOver: Event.create(),
  onNodeMouseMove: Event.create(),
  onNodeMouseLeave: Event.create(),
  onNodeDragStart: Event.create(),
  onNodeDragEnd: Event.create(),
  onNodeDrag: Event.create(),
  onNodeDragEnter: Event.create(),
  onNodeDragLeave: Event.create(),
  onNodeDrop: Event.create(),
  onEdgeClick: Event.create(),
  onEdgeMouseDown: Event.create(),
  onEdgeMouseUp: Event.create(),
  onEdgeDblClick: Event.create(),
  onEdgeContextMenu: Event.create(),
  onEdgeMouseEnter: Event.create(),
  onEdgeMouseOut: Event.create(),
  onEdgeMouseOver: Event.create(),
  onEdgeMouseMove: Event.create(),
  onEdgeMouseLeave: Event.create(),
  onEdgeDragStart: Event.create(),
  onEdgeDragEnd: Event.create(),
  onEdgeDrag: Event.create(),
  onEdgeDragEnter: Event.create(),
  onEdgeDragLeave: Event.create(),
  onEdgeDrop: Event.create(),
};

external castToNode: 'a => ReasonViz__Node.t = "%identity";
external castToEdge: 'a => ReasonViz__Edge.t = "%identity";

[@bs.module "./external/util/base.js"]
external _getItemRoot: 'a => Js.Nullable.t((string, 'b)) = "getItemRoot";
type itemRoot = [ | `Node(ReasonViz__Node.t) | `Edge(ReasonViz__Edge.t)];

let getItemRoot = (a): option(itemRoot) => {
  _getItemRoot(a)
  ->Js.Nullable.toOption
  ->Belt.Option.map(((itemType, item)) =>
      switch (itemType) {
      | "node" => `Node(castToNode(item))
      | "edge" => `Edge(castToEdge(item))
      | _ => assert(false)
      }
    );
};

let cleanableEffects = (g: t, effects) => {
  g.cleanableEffects = List.append(g.cleanableEffects, effects);
};

let emitCanvasEvent = (g: t, eventType, payload) => {
  switch (eventType) {
  | "click" => Event.dispatch(g.events.onCanvasClick, payload)
  | "mousedown" => Event.dispatch(g.events.onCanvasMouseDown, payload)
  | "mouseup" => Event.dispatch(g.events.onCanvasMouseUp, payload)
  | "dblclick" => Event.dispatch(g.events.onCanvasDblClick, payload)
  | "contextmenu" => Event.dispatch(g.events.onCanvasContextMenu, payload)
  | "mouseenter" => Event.dispatch(g.events.onCanvasMouseEnter, payload)
  | "mouseout" => Event.dispatch(g.events.onCanvasMouseOut, payload)
  | "mouseover" => Event.dispatch(g.events.onCanvasMouseOver, payload)
  | "mousemove" => Event.dispatch(g.events.onCanvasMouseMove, payload)
  | "mouseleave" => Event.dispatch(g.events.onCanvasMouseLeave, payload)
  | "dragstart" => Event.dispatch(g.events.onCanvasDragStart, payload)
  | "dragend" => Event.dispatch(g.events.onCanvasDragEnd, payload)
  | "drag" => Event.dispatch(g.events.onCanvasDrag, payload)
  | "dragenter" => Event.dispatch(g.events.onCanvasDragEnter, payload)
  | "dragleave" => Event.dispatch(g.events.onCanvasDragLeave, payload)
  | "drop" => Event.dispatch(g.events.onCanvasDrop, payload)
  | _ => raise(EmitFailure("Failed to emit event:" ++ eventType))
  };
};

let emitNodeEvent = (g: t, eventType, payload) => {
  switch (eventType) {
  | "click" => Event.dispatch(g.events.onNodeClick, payload)
  | "mousedown" => Event.dispatch(g.events.onNodeMouseDown, payload)
  | "mouseup" => Event.dispatch(g.events.onNodeMouseUp, payload)
  | "dblclick" => Event.dispatch(g.events.onNodeDblClick, payload)
  | "contextmenu" => Event.dispatch(g.events.onNodeContextMenu, payload)
  | "mouseenter" => Event.dispatch(g.events.onNodeMouseEnter, payload)
  | "mouseout" => Event.dispatch(g.events.onNodeMouseOut, payload)
  | "mouseover" => Event.dispatch(g.events.onNodeMouseOver, payload)
  | "mousemove" => Event.dispatch(g.events.onNodeMouseMove, payload)
  | "mouseleave" => Event.dispatch(g.events.onNodeMouseLeave, payload)
  | "dragstart" => Event.dispatch(g.events.onNodeDragStart, payload)
  | "dragend" => Event.dispatch(g.events.onNodeDragEnd, payload)
  | "drag" => Event.dispatch(g.events.onNodeDrag, payload)
  | "dragenter" => Event.dispatch(g.events.onNodeDragEnter, payload)
  | "dragleave" => Event.dispatch(g.events.onNodeDragLeave, payload)
  | "drop" => Event.dispatch(g.events.onNodeDrop, payload)
  | _ => raise(EmitFailure("Failed to emit event:" ++ eventType))
  };
};

let emitEdgeEvent = (g: t, eventType, payload) => {
  switch (eventType) {
  | "click" => Event.dispatch(g.events.onEdgeClick, payload)
  | "mousedown" => Event.dispatch(g.events.onEdgeMouseDown, payload)
  | "mouseup" => Event.dispatch(g.events.onEdgeMouseUp, payload)
  | "dblclick" => Event.dispatch(g.events.onEdgeDblClick, payload)
  | "contextmenu" => Event.dispatch(g.events.onEdgeContextMenu, payload)
  | "mouseenter" => Event.dispatch(g.events.onEdgeMouseEnter, payload)
  | "mouseout" => Event.dispatch(g.events.onEdgeMouseOut, payload)
  | "mouseover" => Event.dispatch(g.events.onEdgeMouseOver, payload)
  | "mousemove" => Event.dispatch(g.events.onEdgeMouseMove, payload)
  | "mouseleave" => Event.dispatch(g.events.onEdgeMouseLeave, payload)
  | "dragstart" => Event.dispatch(g.events.onEdgeDragStart, payload)
  | "dragend" => Event.dispatch(g.events.onEdgeDragEnd, payload)
  | "drag" => Event.dispatch(g.events.onEdgeDrag, payload)
  | "dragenter" => Event.dispatch(g.events.onEdgeDragEnter, payload)
  | "dragleave" => Event.dispatch(g.events.onEdgeDragLeave, payload)
  | "drop" => Event.dispatch(g.events.onEdgeDrop, payload)
  | _ => raise(EmitFailure("Failed to emit event:" ++ eventType))
  };
};

let preItem = ref(None);

let handleMouseMove = (~g, ~item, ~event) => {
  let emitNodeEvent = (g, eventType, event, node) => {
    let event = Js.Obj.assign(event, {"type": "mouse" ++ eventType});
    emitNodeEvent(g, "mouse" ++ eventType, (event, node));
    if (g.dragging) {
      let event = Js.Obj.assign(event, {"type": "drag" ++ eventType});
      emitNodeEvent(g, "drag" ++ eventType, (event, node));
    };
  };

  let emitEdgeEvent = (g, eventType, event, edge) => {
    let event = Js.Obj.assign(event, {"type": "mouse" ++ eventType});
    emitEdgeEvent(g, "mouse" ++ eventType, (event, edge));
    if (g.dragging) {
      let event = Js.Obj.assign(event, {"type": "drag" ++ eventType});
      emitEdgeEvent(g, "drag" ++ eventType, (event, edge));
    };
  };

  if (preItem^ != item) {
    switch (preItem^) {
    | Some(`Node(node)) => emitNodeEvent(g, "leave", event, node)
    | Some(`Edge(edge)) => emitEdgeEvent(g, "leave", event, edge)
    | None => ()
    };

    switch (item) {
    | Some(`Node(node)) => emitNodeEvent(g, "enter", event, node)
    | Some(`Edge(edge)) => emitEdgeEvent(g, "enter", event, edge)
    | None => ()
    };
  };

  preItem := item;
};

let canvasHandler = (g: t, e) => {
  let canvas = g.canvas;
  let pixelRatio = Canvas.getInt(canvas, "pixelRatio");
  let target = e##target;
  let eventType: string = [%raw "e.type"];

  let canvasX = e##x / pixelRatio;
  let canvasY = e##y / pixelRatio;

  let _point = {"x": float_of_int(canvasX), "y": float_of_int(canvasY)};

  let point =
    if (Util.Graphic.isViewportChanged(~group=g.group)) {
      View.getPointByCanvas(g, _point);
    } else {
      _point;
    };

  let attrs = {
    "canvasX": canvasX,
    "canvasY": canvasY,
    "x": point##x,
    "y": point##y,
  };

  let event: ReasonViz__GraphTypes.event = Js.Obj.assign(e, attrs);
  let mouseMove = eventType == "mousemove";

  if (target == canvas) {
    mouseMove ? handleMouseMove(~g, ~item=None, ~event) : ();
    emitCanvasEvent(g, eventType, (event, canvas));
  } else {
    let item = getItemRoot(target);
    let entranceEvent =
      Util.String.indexOf(eventType, "leave") > (-1)
      || Util.String.indexOf(eventType, "enter") > (-1);

    mouseMove ?  handleMouseMove(~g, ~item, ~event) :();

    if (!entranceEvent) {
      switch (item) {
      | Some(`Node(node)) => emitNodeEvent(g, eventType, (event, node))
      | Some(`Edge(edge)) => emitEdgeEvent(g, eventType, (event, edge))
      | None => ()
      };
    };
  };

  switch (eventType) {
  | "dragstart" => g.dragging = true
  | "dragend" => g.dragging = false
  | _ => ()
  };

  ();
};

let bindEvents = (g: t) => {
  let canvas = g.canvas;
  let el = canvas->Canvas.getEl("el");
  let window: element = [%raw "window"];
  let originHandler = () => Js.log("origin");
  let wheelHandler = () => Js.log("wheel");

  let canvasHandler = canvasHandler(g);

  [
    "click",
    "mousedown",
    "mouseup",
    "dblclick",
    "contextmenu",
    "mouseenter",
    "mouseout",
    "mouseover",
    "mousemove",
    "mouseleave",
    "dragstart",
    "dragend",
    "drag",
    "dragenter",
    "dragleave",
    "drop",
  ]
  |> List.map(event => {
       Canvas.on(canvas, event, canvasHandler);
       () => Canvas.off(canvas, event, canvasHandler);
     })
  |> cleanableEffects(g);

  ["DOMMouseScroll", "mousewheel"]
  |> List.map(event => {
       let res = Util.addEventListener(el, event, wheelHandler);
       () => res##remove();
     })
  |> cleanableEffects(g);

  ["keydown", "keyup"]
  |> List.map(event => {
       let res = Util.addEventListener(window, event, originHandler);
       () => res##remove();
     })
  |> cleanableEffects(g);

  ();
};

let clean = (g: t) => List.iter(e => e(), g.cleanableEffects);
