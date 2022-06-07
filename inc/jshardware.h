JsSysTime currentSystemTime;


JsSysTime jshGetSystemTime() { return currentSystemTime; }
JsVarFloat jshGetMillisecondsFromTime(JsSysTime time) { return (JsVarFloat)time; }
