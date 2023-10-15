void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}
class DotMatrix {
public:
    DotMatrix(int Raw){
        this->Raw = Raw
    }
    void print(){
        Serial.write("hello")
    }
private:
    int Raw;
};