open ReasonViz__Types;
module Styles = ReasonViz__Drawing_Styles;

type nodeLabel = {
  mutable styles: Js.Dict.t(string),
  mutable position: labelPosition,
  mutable offset: int,
};

let nodeLabel: nodeLabel = {
  styles:
    Styles.(
      make([fill("#595959"), textAlign(`center), textBaseline(`middle)])
    ),
  position: `center,
  offset: 5,
};

type node = {
  mutable styles: list(Styles.rule),
  mutable size,
};

let node: node = {
  styles: Styles.[fill("#fff"), stroke("#333")],
  size: {
    width: 40,
    height: 40,
  },
};
