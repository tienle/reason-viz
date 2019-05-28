module type DrawableShape = {
  let updatePosition:
    (ReasonViz__Node_Type.t('a), option(ReasonViz__Types.point)) => unit;
  let draw: (ReasonViz__Node_Type.t('a)) => ReasonViz__Canvas.Shape.t;
}

let getModule: (ReasonViz__Types.shape('a)) => (module DrawableShape);
