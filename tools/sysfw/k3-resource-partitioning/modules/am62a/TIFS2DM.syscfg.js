
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "TIFS to DM communication",
  "Security": "Non Secure",
  "displayName": "TIFS to DM communication",
  "hostId": 251,
  "hostName": "TIFS2DM"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
