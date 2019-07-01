module Styles = ReasonViz__Drawing_Styles;
module Canvas = ReasonViz__Canvas;
module GraphTypes = ReasonViz__GraphTypes;
open ReasonViz__Types;

module Label = {
  open GraphTypes.EdgeLabel;

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
  type vertex = GraphTypes.vertex;
  type t = GraphTypes.edgeModel;

  let vertexToPoint: vertex => point =
    fun
    | `Point(point) => point
    | `Node(node) => {x: node.x, y: node.y}
    | `NodeId(id) => raise(NodeNotFound(id));

  let default =
    ref({
      GraphTypes.id: "",
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
      )
      : GraphTypes.edgeModel => {
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

type t = GraphTypes.edge;

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
  let state = Js.Dict.empty();
  let edge: t = {model, group, shape, state};
  Canvas.Group.set(group, "id", model.id);
  Canvas.Group.set(group, "item", ("edge", edge));
  edge;
};

let setState = (e: t, key, value) => {
  Js.Dict.set(e.state, key, value);
};
