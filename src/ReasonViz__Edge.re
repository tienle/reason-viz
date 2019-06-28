module Styles = ReasonViz__Drawing_Styles;
module Canvas = ReasonViz__Canvas;
open ReasonViz__Types;

module Label = {
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

  let default =
    ref({
      position: `center,
      autoRotate: false,
      refX: 0,
      refY: 0,
      styles:
        StylesList.make(
          Styles.[fill("#333"), textAlign(`center), textBaseline(`middle)],
        ),
    });

  let make =
      (
        ~text,
        ~position=default^.position,
        ~autoRotate=default^.autoRotate,
        ~refX=default^.refX,
        ~refY=default^.refY,
        ~styles=[],
        (),
      ) => {
    let styles = StylesList.make(styles);
    {text, cfg: Some({position, autoRotate, refX, refY, styles})};
  };
};

module UniqueId =
  ReasonViz__Utils.UniqueId.Make({
    let prefix = "edge";
  });

module Model = {
  type vertex = [
    | `Point(point)
    | `NodeId(string)
    | `Node(ReasonViz__Node.t)
  ];

  type t = {
    id: string,
    mutable source: vertex,
    mutable target: vertex,
    mutable sourcePoint: point,
    mutable targetPoint: point,
    shape: string,
    styles: StylesList.t,
    label: option(Label.t),
    sourceAnchor: string,
    targetAnchor: string,
    controlPoints: array(point),
    linkCenter: bool,
    size: int,
  };

  let vertexToPoint: vertex => point =
    fun
    | `Point(point) => point
    | `Node(node) => {x: node.x, y: node.y}
    | `NodeId(id) => raise(NodeNotFound(id));

  let default =
    ref({
      id: "",
      source: `NodeId(""),
      target: `NodeId(""),
      sourcePoint: {
        x: 0.0,
        y: 0.0,
      },
      targetPoint: {
        x: 0.0,
        y: 0.0,
      },
      shape: "line",
      styles: StylesList.make(Styles.[stroke("#333")]),
      label: None,
      sourceAnchor: "",
      targetAnchor: "",
      controlPoints: [||],
      linkCenter: false,
      size: 1,
    });

  let make =
      (
        ~id=?,
        ~source,
        ~target,
        ~styles=[],
        ~shape=default^.shape,
        ~label=?,
        ~sourceAnchor=default^.sourceAnchor,
        ~targetAnchor=default^.targetAnchor,
        ~controlPoints=default^.controlPoints,
        ~linkCenter=default^.linkCenter,
        ~size=default^.size,
        (),
      ) => {
    let styles = StylesList.make(styles);
    let id = Belt.Option.getWithDefault(id, UniqueId.generate());
    let sourcePoint = {x: 0.0, y: 0.0};
    let targetPoint = {x: 0.0, y: 0.0};

    {
      id,
      source,
      target,
      sourcePoint,
      targetPoint,
      styles,
      shape,
      label,
      sourceAnchor,
      targetAnchor,
      controlPoints,
      linkCenter,
      size,
    };
  };
};

type t = {
  model: Model.t,
  group: Canvas.Group.t,
  mutable shape: Canvas.Shape.t,
};

let make = (~nodesMap, ~parentGroup, ~model: Model.t) => {
  let group = Canvas.Group.make(parentGroup);

  let translateToNode =
    fun
    | `NodeId(id) => {
        let node = Js.Dict.get(nodesMap, id);
        switch (node) {
        | Some(node) => `Node(node)
        | None => raise(NodeNotFound(id))
        };
      }
    | point => point;

  model.source = translateToNode(model.source);
  model.target = translateToNode(model.target);

  let shape = Canvas.Shape.empty();
  let edge = {model, group, shape};
  Canvas.Group.set(group, "id", model.id);
  Canvas.Group.set(group, "item", ("edge", edge));
  edge;
};
