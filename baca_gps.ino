double get_lat()
{   
  if (gps.location.isValid())
  {
   return gps.location.lat();
  }
  else return 0;
}


double get_long()
{   
  if (gps.location.isValid())
  {
   return gps.location.lng();
  }
  else return 0;
}
