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
