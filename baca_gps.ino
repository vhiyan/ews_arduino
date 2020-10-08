double get_lat()
{   
  if (gps.location.isValid())
  {
   return gps.location.lat()*10000000;
  }
  else return 0;
}


double get_long()
{   
  if (gps.location.isValid())
  {
   return gps.location.lng()*10000000;
  }
  else return 0;
}
