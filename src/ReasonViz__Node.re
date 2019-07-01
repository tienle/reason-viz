module Styles = ReasonViz__Drawing_Styles;
module Canvas = ReasonViz__Canvas;
module GraphTypes = ReasonViz__GraphTypes;
open ReasonViz__Types;

module Label = {
  open GraphTypes.NodeLabel;

  let default =
    ref({
      styles:
        StylesList.make(
          Styles.[fill("#333"), textAlign(`center), textBaseline(`middle)],
        ),
      position: `center,
      offset: 5,
    });

  let make =
      (
        ~text,
        ~position=default^.position,
        ~offset=default^.offset,
        ~styles=[],
        (),
      ) => {
    let styles = StylesList.make(styles);
    {text, cfg: Some({position, offset, styles})};
  };
};

module Model = {
  open GraphTypes.NodeModel;

  let default =
    ref({
      id: "default",
      props: GraphTypes.PropsList.make([]),
      styles: StylesList.make(Styles.[fill("#fff"), stroke("#333")]),
      anchorPoints: [],
      label: None,
      shape: "circle",
      size: {
        width: 0,
        height: 0,
      },
    });

  let make = (~id, ~props, ~styles, ~anchorPoints=[], ~shape, ~label=?, ()) => {
    let props = GraphTypes.PropsList.make(props);
    let styles = StylesList.make(styles);
    let (w, h) = props.getExn("size") |> GraphTypes.ShapeValue.toPairInt;
    let size = {width: w, height: h};

    {id, props, styles, anchorPoints, label, shape, size};
  };
};

type t = GraphTypes.node;

let make = (~parentGroup, ~model: GraphTypes.NodeModel.t) => {
  let group = Canvas.Group.make(parentGroup);
  Canvas.Group.set(group, "id", model.id);
  let x = GraphTypes.ShapeValue.(model.props.getExn("x") |> toFloat);
  let y = GraphTypes.ShapeValue.(model.props.getExn("y") |> toFloat);
  let shape = Canvas.Shape.empty();
  let state = Js.Dict.empty();
  let node: t = {model, group, shape, x, y, state, edges: []};
  Canvas.Group.set(group, "item", ("node", node));
  node;
};

let setState = (n: t, key, value) => {
  Js.Dict.set(n.state, key, value);
};
