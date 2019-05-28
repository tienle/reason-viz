/* const node = new Node({ */
/*   id: '',            */
/*   shape: '', */
/*   x: 0, */
/*   y: 0, */
/*   size: [], */
/*   style: {...}, */
/*   label: 'label text', */
/*   labelCfg: { */
/*     position: 'top', */
/*     style: {...} */
/*   }, */
/*   anchorPoints: [] */
/*  }); */
open ReasonViz__Types;
open ReasonViz__Node_Type;
module Canvas = ReasonViz__Canvas;
module Data = ReasonViz__Data;

/* include ReasonViz__Item; */

module UniqueId =
  ReasonViz__Utils.UniqueId.Make({
    let prefix = "node";
  });

let draw = node => {
  module S = (val ReasonViz__Shape.getModule(node.model.shape));

  Canvas.Group.clear(node.group);

  S.updatePosition(node, None);
  let shape = S.draw(node);
  node.keyShape = Some(shape);
  ();
};

let create =
    (
      ~model: Data.node('a),
      ~parentGroup,
      ~animate=false,
      ~visible=true,
      ~event=true,
      (),
    ) => {
  let id = Belt.Option.getWithDefault(model.id, UniqueId.getUniqueId());
  let group = Canvas.Group.make(parentGroup);
  Canvas.Group.set(group, "id", id);

  let node = {
    id,
    group,
    animate,
    visible,
    event,
    states: [],
    model,
    anchors: [],
    edges: [],
    status: [],
    keyShape: None,
  };

  Canvas.Group.set(group, "item", node);
  draw(node);
  node;
};

/* let addEdge = edge => edge; */
