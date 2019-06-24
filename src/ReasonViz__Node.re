module Styles = ReasonViz__Drawing_Styles;
module Canvas = ReasonViz__Canvas;
open ReasonViz__Types;

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

module Label = {
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
  type t = {
    id: string,
    props: PropsList.t,
    styles: StylesList.t,
    anchorPoints: list((float, float)),
    label: option(Label.t),
    shape: string,
  };

  let default =
    ref({
      id: "default",
      props: PropsList.make([]),
      styles: StylesList.make(Styles.[fill("#fff"), stroke("#333")]),
      anchorPoints: [],
      label: None,
      shape: "circle",
    });

  let make = (~id, ~props, ~styles, ~anchorPoints=[], ~shape, ~label=?, ()) => {
    let props = PropsList.make(props);
    let styles = StylesList.make(styles);

    {id, props, styles, anchorPoints, label, shape};
  };
};

type t = {
  model: Model.t,
  group: Canvas.Group.t,
  mutable shape: Canvas.Shape.t,
  mutable x: float,
  mutable y: float,
};

let make = (~parentGroup, ~model: Model.t) => {
  let group = Canvas.Group.make(parentGroup);
  Canvas.Group.set(group, "id", model.id);
  let x = ShapeValue.(model.props.getExn("x") |> toFloat);
  let y = ShapeValue.(model.props.getExn("y") |> toFloat);
  let shape = Canvas.Shape.empty();

  {model, group, shape, x, y};
};
