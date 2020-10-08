bool timer(unsigned int id,unsigned long minidetik)
{
  currentTime[id] = millis();
  if(currentTime[id]-previousTime[id]>=minidetik)
  {
    previousTime[id] = millis();
    return true;
  }
  else 
    return false;
}
