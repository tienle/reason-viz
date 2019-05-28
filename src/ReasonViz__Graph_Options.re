open Belt;
/** [t] is the type representating creation options for a graph */
/* type padding = { */
/*   left: int, */
/*   top: int, */
/*   right: int, */
/*   bottom: int, */
/* }; */
/*  */
/* type mode; */
/* type easing = */
/*   | EaseLinear */
/*   | CubicBezier; */
/* type nodeConfig; */
/* type edgeConfig; */
/* type plugin; */
/* type animateCfg = { */
/*   onFrame: float => unit, */
/*   duration: int, */
/*   easing, */
/* }; */
/*  */
/* /* TODO Styles */ */
/* type nodeStyle; */
/* type edgeStyle; */
/*  */
/* type data; */

type t = {
  containerId: string,
  width: int,
  height: int,
  /* fitViewPadding: padding, */
  /* groupByTypes: bool, */
  /* autoPaint: bool, */
  renderer: ReasonViz__Canvas.renderer,
  pixelRatio: option(int),
  /* mode: list(mode), */
  /* nodeStyle, */
  /* edgeStyle, */
  /* defaultNode: nodeConfig, */
  /* defaultEdge: edgeConfig, */
  /* plugins: list(plugin), */
  /* data, */
  /* minZoom: float, */
  /* maxZoom: float, */
  /* event: bool, */
  /* directed: bool, */
  /* animate: bool, */
  /* animateCfg, */
};

/* ~data, */
/* ~fitViewPadding={left: 0, top: 0, right: 0, bottom: 0}, */
/* ~groupByTypes=true, */
/* ~autoPaint=true, */
/* ~renderer=Canvas, */
/* ~mode=[], */
/* ~plugins=[], */
/* ~minZoom=0.2, */
/* ~maxZoom=10, */
/* ~event=true, */
/* ~directed=false, */
/* ~animate=false, */
/* ~animateCfg=?, */
/* ~nodeStyle=?, */
/* ~edgeStyle=?, */
/* ~defaultNode=?, */
/* ~defaultEdge=?, */
/* (), */

let create = (~containerId, ~width, ~height, ~renderer=?, ~pixelRatio=?, ()) => {
  containerId,
  width,
  height,
  renderer: Option.getWithDefault(renderer, ReasonViz__Canvas.Canvas),
  pixelRatio,
};
