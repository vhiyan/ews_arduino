float read_lux()
{
  float lux = lightMeter.readLightLevel(true);
  if (lux > 40000.0)lightMeter.setMTreg(32);
  if (lux > 10.0 && lux <=40000)lightMeter.setMTreg(69);
  if (lux <= 10.0 && lux > 0)lightMeter.setMTreg(138);
  return lux;
}
