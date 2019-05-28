type group;
type state;
open ReasonViz__Types;

type item = {
  id: string,
  group,
  animate: bool,
  visible: bool,
  event: bool,
  states: list(state),
};

/* let create = */
/*     (~id, ~model, ~group, ~animate=false, ~visible=true, ~event=true, ()) => */
/* { */
/*   id: 'foo', */
/*   model: model, */
/*   group: group, */
/*   animate, */
/*   visible, */
/*   event, */
/*   keyShape, */
/*   states */
/* } */
