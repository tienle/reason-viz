include ReasonViz__Single_Shape.SingleShape;
module Canvas = ReasonViz__Canvas;

let unpackShape = shape => {
  switch (shape) {
  | `Circle(c) => c
  | _ => assert(false)
  };
};

let updatePosition =
    (
      node: ReasonViz__Node_Type.t('a),
      position: option(ReasonViz__Types.point),
    ) => {
  let model = unpackShape(node.model.shape);

  switch (position) {
  | Some({x, y}) =>
    model.x = x;
    model.y = y;
  | None => ()
  };

  let group = node.group;
  Canvas.Group.resetMatrix(group);
  Canvas.Group.translate(group, model.x, model.y);
};

/* TODO */
let drawShape = (node: ReasonViz__Node_Type.t('a)) => {
  let shape = Canvas.Group.addShape(node.group, "text", {"text": "hello"});
  Canvas.Shape.set(shape, "className", "node-shape");
  shape;
};

let getLabelStyleByPosition = (node: ReasonViz__Node_Type.t('a)) => {
  let size = node.model.size;
  let width = size.width;
  let height = size.height;
  let offset =
    switch (node.model.label) {
    | Some(label) => label.cfg.offset
    | None => 0
    };
  let position =
    switch (node.model.label) {
    | Some(label) => label.cfg.position
    | None => `center
    };
  let text =
    switch (node.model.label) {
    | Some(label) => label.text
    | None => ""
    };

  let textAttr = ("text", text);

  let styles =
    switch (position) {
    | `center => [textAttr, ("x", "0"), ("y", "0")]
    | `top => [
        textAttr,
        ("x", "0"),
        ("y", string_of_int(0 - height / 2 - offset)),
        ("textBaseline", "bottom"),
      ]
    | `bottom => [
        textAttr,
        ("x", "0"),
        ("y", string_of_int(height / 2 + offset)),
        ("textBaseline", "top"),
      ]
    | `left => [
        textAttr,
        ("x", string_of_int(0 - width / 2 - offset)),
        ("y", "0"),
        ("textAlign", "right"),
      ]
    | `right => [
        textAttr,
        ("x", string_of_int(width / 2 + offset)),
        ("y", "0"),
        ("textAlign", "left"),
      ]
    };

  styles |> Js.Dict.fromList;
};

let getLabelStyle = node => {
  let calculateStyles = getLabelStyleByPosition(node);
  let labelCfgStyles =
    switch (node.model.label) {
    | Some(label) => label.cfg.styles
    | None => Js.Dict.empty()
    };

  ReasonViz__Utils.mergeDict([
    ReasonViz__Default_Data.nodeLabel.styles,
    calculateStyles,
    labelCfgStyles,
  ]);
};

let drawLabel = node => {
  let labelStyle = getLabelStyle(node);

  let label =
    Canvas.Group.addShape(node.group, "text", {"attrs": labelStyle});
  Canvas.Shape.set(label, "className", "node-label");
  ();
};

let draw = (node: ReasonViz__Node_Type.t('a)) => {
  let shape = drawShape(node);

  if (Belt.Option.isSome(node.model.label)) {
    drawLabel(node);
  };

  shape;
};
