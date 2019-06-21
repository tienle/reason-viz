const BaseUtil = require('./base');
const G = require('@antv/g/lib');

const PathUtil = {
  catmullRomToBezier(data) {
    const splinePath = G.PathUtil.catmullRomToBezier(data);
    const res = [];

    for (let i = 0; i < splinePath.length; i++) {
      let [a, ...b] = splinePath[i];
      res.push([a, b]);
    }

    return res;
  },
};
module.exports = BaseUtil.mix({}, BaseUtil, PathUtil);

