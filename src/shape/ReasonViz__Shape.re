open ReasonViz__Types;
module type DrawableShape = {
  let updatePosition:
    (ReasonViz__Node_Type.t('a), option(ReasonViz__Types.point)) => unit;
  let draw: ReasonViz__Node_Type.t('a) => ReasonViz__Canvas.Shape.t;
};

let getModule = (shapeType: shape('a)) => {
  switch (shapeType) {
  | `Circle(_) => ((module ReasonViz__Shape_Circle): (module DrawableShape))
  | _ => ((module ReasonViz__Shape_Circle): (module DrawableShape))
  };
};
