open ReasonViz__Types;
module Canvas = ReasonViz__Canvas;

type t('a) = {
  canvas: Canvas.t,
  group: Canvas.Group.t,
  mutable itemMap: StringHash.t(ReasonViz__Node_Type.t('a)),
  mutable nodes: list(ReasonViz__Node_Type.t('a)),
  /* mutable edges: list(edge), */
};

let create: (~graphOptions: ReasonViz__Graph_Options.t) => t('a);

let parse: (t('a), ~data: ReasonViz__Data.t('a)) => t('a);
let render: t('a) => unit;
