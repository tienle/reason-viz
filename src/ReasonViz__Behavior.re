open ReasonViz__Types;
module C = ReasonViz__Canvas;
module E = ReasonViz__Event;
module GT = ReasonViz__GraphTypes;
module G = ReasonViz__Graph;
module Util = ReasonViz__Utils;

module CleanableEffect = {
  let effects = ref([]);

  let useEffects = eff => {
    effects := List.append(effects^, eff);
  };

  let clean = () => List.iter(e => e(), effects^);
};

module ClickToSelect =
       (Styles: {
          let selected: StylesList.t;
          let normal: StylesList.t;
        }) => {
  include CleanableEffect;

  let deselectAllNodes = g => {
    let deselectNode = G.setNodeState(~key="state", ~value="");
    G.findNodesByState(g, "state", "selected") |> List.iter(deselectNode);
  };

  let onNodeClick = ((_, n: GT.node)) => {
    deselectAllNodes(n.graph);

    let state =
      switch (G.getNodeState(n, ~key="state")) {
      | None => "selected"
      | Some(state) => state == "selected" ? "" : "selected"
      };

    G.setNodeState(~key="state", ~value=state, n);
  };

  let onCanvasClick = (g, (_, _)) => {
    deselectAllNodes(g);
    G.paint(g);
  };

  let onNodeStateUpdated = ((node: GT.node, key, value, _oldValue)) => {
    let shape = node.shape;

    switch (key, value) {
    | ("state", "selected") =>
      C.Shape.attrs(shape, Styles.selected |> Util.castToJsObj)
    | ("state", _) => C.Shape.attrs(shape, Styles.normal |> Util.castToJsObj)
    | _ => ()
    };

    G.paint(node.graph);
  };

  let activate = (g: GT.t) => {
    [
      E.subscribe(g.events.onNodeClick, onNodeClick),
      E.subscribe(g.events.onCanvasClick, onCanvasClick(g)),
      E.subscribe(g.events.onNodeStateUpdated, onNodeStateUpdated),
    ]
    |> useEffects;
  };

  let deactivate = _ => {
    clean();
  };
};

module DragNode = (Styles: {let draggingStyle: Js.t({..});}) => {
  include CleanableEffect;

  type point = {
    x: int,
    y: int,
  };

  let revertOriginal = ref(() => ());

  let onNodeDragStart = ((e, node: GT.node)) => {
    let originalAttrs = C.Shape.getAttrs(node.shape);
    revertOriginal :=
      (
        () => {
          C.Shape.attrs(node.shape, originalAttrs);
        }
      );
  };

  let onNodeDrag = ((e, node: GT.node)) => {
    node.x = float_of_int(e##x);
    node.y = float_of_int(e##y);

    C.Shape.attrs(node.shape, Styles.draggingStyle);

    module Shape = (val ReasonViz__NodeShape.get(node.model.shape));

    Shape.translateToPosition(node);
    node.edges
    |> List.iter((edge: GT.edge) => {
         module EdgeShape = (val ReasonViz__EdgeShape.get(edge.model.shape));
         EdgeShape.draw(edge);
       });
    G.paint(node.graph);
  };

  let onNodeDragEnd = ((_e, node: GT.node)) => {
    revertOriginal^();
    G.paint(node.graph);
  };

  let onOutOfRange = ((_e, _canvas)) => {
    ();
  };

  let activate = (g: GT.t) => {
    [
      E.subscribe(g.events.onNodeDragStart, onNodeDragStart),
      E.subscribe(g.events.onNodeDrag, onNodeDrag),
      E.subscribe(g.events.onNodeDragEnd, onNodeDragEnd),
      E.subscribe(g.events.onCanvasMouseLeave, onOutOfRange),
    ]
    |> useEffects;
    ();
  };

  let deactivate = _ => {
    clean();
  };
};
