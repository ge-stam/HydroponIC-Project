#ifndef WaterPump_h
#define WaterPump_h
    
class WaterPump{

  public:

  WaterPump();
  void Init_WaterPump();
  void Set_WP_ON();
  void Set_WP_OFF(); 
  bool Get_WP_State();

  private:
  bool WaterPump_state = false;

  };

#endif