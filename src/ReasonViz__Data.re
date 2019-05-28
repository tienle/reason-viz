open ReasonViz__Types;

module Styles = ReasonViz__Drawing_Styles;
module Default = ReasonViz__Default_Data;

module NodeLabel = {
  type cfg = {
    position: labelPosition,
    offset: int,
    styles: Js.Dict.t(string),
  };

  type t = {
    text: string,
    cfg,
  };

  let makeCfg =
      (
        ~position=Default.nodeLabel.position,
        ~offset=Default.nodeLabel.offset,
        ~styles=[],
        (),
      ) => {
    let styles = Styles.make(styles);
    {position, offset, styles};
  };
};

type node('a) = {
  shape: shape('a),
  styles: Js.Dict.t(string),
  size,
  id: option(string),
  label: option(NodeLabel.t),
};

type edge = {
  source: string,
  target: string,
  id: option(string),
};

type t('a) = {
  nodes: list(node('a)),
  /* edges: list(edge), */
};

let node =
    (
      ~shape: shape('a),
      ~styles=[],
      ~size=Default.node.size,
      ~id: option(string)=?,
      ~label: option(NodeLabel.t)=?,
      (),
    )
    : node('a) => {
  let styles = Styles.make(styles);

  {shape, styles, size, id, label};
};

let edge = (~source, ~target, ~id=?, ()) => {
  {source, target, id};
};
