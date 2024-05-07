#include <iostream>
using namespace std;
# define M 1000000007

template <typename T>
class Stack{ 
private:
    T* data;    //数据
    int top;    //栈顶
    int capacity;   //容量
public:
    Stack(int size){
        data = new T[size]();
        top = 0;
        capacity = size;
    }
    bool empty(){
        return (top==0);
    }
    T pop(){
        if(top==0)return T();
        return data[--top];
    }
    void push(T x){
        data[top++] = x;
    }
    //整数入栈
    void Intpush(int x){
        data[top].TopDeg = 0;
        data[top].Coe[0] = x;
        top++;
    }
    //x入栈
    void Xpush(){
        data[top].TopDeg = 1;
        data[top].Coe[0] = 0;
        data[top].Coe[1] = 1;
        top++;
    }
    T get_top(){
        if(top==0)return T();
        return data[top-1];
    }
    bool IsEmpty(){
        return top==0;
    }
    bool IsFull(){
        return top==capacity;
    }
    void init(){
        top = 0;
    }
    //弹出栈顶元素（不返回具体值）
    void pope(){
        if(top==0)return;
        top--;
    }
    //计算(inplace)
    void calc(char op){
        switch(op){
            case '+':{
                data[top-2].add(data[top-1]);   //将栈顶两个元素相加并将结果存入栈顶第二个元素
                break;
            }
            case '-':{
                data[top-2].minus(data[top-1]); //将栈顶两个元素相减并将结果存入栈顶第二个元素
                break;
            }
            case '*':{
                data[top-2].multiply(data[top-1]);  //将栈顶两个元素相乘并将结果存入栈顶第二个元素
                break;
            }
            case '^':{
                data[top-2].power(data[top-1]); //将栈顶两个元素相乘并将结果存入栈顶第二个元素
                break;
            }
        }
        pope(); //弹出栈顶元素
    }
};

//多项式类
class Polynomial{
public:
    int TopDeg; //最高次数
    int Coe[70]; //系数数列

    Polynomial(){
        TopDeg=0;
    }
    
    //打印多项式
    void print() {
        for(int i = TopDeg; i >= 0; i--){
            printf("%d ", Coe[i]);
        }
        printf("\n");
    }

    //拷贝副本
    Polynomial(const Polynomial& other) {
        TopDeg = other.TopDeg;
        for (int i = 0; i <= TopDeg; i++) {
            Coe[i] = other.Coe[i];
        }
    }

    // 拷贝赋值运算符
    Polynomial& operator=(const Polynomial& other) {
        TopDeg = other.TopDeg;
        for(int i=0;i<=TopDeg;i++){
            Coe[i] = other.Coe[i];
        }
        return *this;
    }

    //加法
    void add(const Polynomial &x){
        int Deg = max(TopDeg,x.TopDeg);
        for(int i=0;i<=Deg;i++){
            Coe[i] = (i<=TopDeg?Coe[i]:0) + (i<=x.TopDeg?x.Coe[i]:0);
            if(Coe[i]>M)Coe[i]-=M;
        }
        TopDeg = Deg;
        while(TopDeg>0 && Coe[TopDeg]==0)TopDeg--;
    }

    //减法
    void minus(const Polynomial &x){
        int Deg = max(TopDeg,x.TopDeg);
        int temp = TopDeg;
        for(int i=0;i<=Deg;i++){
            Coe[i] = (i<=temp?Coe[i]:0) - (i<=x.TopDeg?x.Coe[i]:0);
            if(Coe[i]<0)Coe[i]+=M;
        }
        TopDeg = Deg;
        while(TopDeg>0 && Coe[TopDeg]==0)TopDeg--;
    }

    //乘法
    void multiply(const Polynomial &x){
        if(this->Coe[0]==0&&this->TopDeg==0){
            TopDeg = 0;
            this->Coe[0] = 0;
            return;
        }
        int Deg = TopDeg + x.TopDeg;
        int* temp = new int[Deg+1]();
        for(int i=0;i<=TopDeg;i++){
            for(int j=0;j<=x.TopDeg;j++){
                temp[i+j] = ((long long)(Coe[i])*(long long)(x.Coe[j]) + temp[i+j])%M;
            }
        }
        TopDeg = Deg;
        for(int i=0;i<=TopDeg;i++){
            Coe[i] = temp[i];
        }
        delete[] temp;
    }

    //乘方（最多四次，未使用快速幂）
    void power(const Polynomial& xx) {
        if(xx.TopDeg == 0 && xx.Coe[0] == 0) {
            TopDeg = 0;
            Coe[0] = 1;
            return;
        }
        int x = xx.Coe[0];
        Polynomial base(*this); // 使用拷贝构造函数创建base的副本
        while(x>1){
            this->multiply(base);
            x--;
        }
    }
};

const char pri[7][7] = { //运算符优先等级 [栈顶][当前]
//          |--------- 当前运算符 ----------|
//           +    -    *    ^    (    )   \0
/* -- +  */ '>', '>', '<', '<', '<', '>', '>',
/* 栈 -  */ '>', '>', '<', '<', '<', '>', '>',
/* 顶 *  */ '>', '>', '>', '<', '<', '>', '>',
/* 运 ^  */ '>', '>', '>', '>', '<', '>', '>',
/* 算 (  */ '<', '<', '<', '<', '<', '=', ' ',
/* 符 )  */ ' ', ' ', ' ', ' ', ' ', ' ', ' ',
/* -- \0 */ '<', '<', '<', '<', '<', ' ', '='
};

//判断是否为数字
bool IsDigit(char x){
    if(x >= 48 && x <= 57)return true;
    return false;
}

//加括号
string AddMultiple(string init){
    int l = init.length();
    bool* AddHere = new bool[l-1]();    //记录是否需要加乘号
    for(int i=0;i<l-1;i++){
        //如果前一个是数字，后一个是x或者左括号，或者前一个是x或者左括号，后一个是数字或者左括号，或者前一个是右括号，后一个是x或者左括号，或者前一个是右括号，后一个是左括号
        if((IsDigit(init[i])&&(init[i+1]=='x'))||((init[i]=='x')&&(init[i+1]=='('))||((IsDigit(init[i]))&&(init[i+1]=='('))||((init[i]==')')&&(init[i+1]=='x'))||((init[i]==')')&&(init[i+1]=='(')))AddHere[i] = true;
    }
    string temp;
    int indexInInit = 0;
    while(indexInInit<=l-1){
        if(!AddHere[indexInInit]){
            temp.push_back(init[indexInInit]);
            indexInInit++;
        }
        else{
            temp.push_back(init[indexInInit]);
            temp.push_back('*');
            indexInInit++;
        }
    }
    return temp;
}

int optr2rank ( char op ) { //由运算符转译出编号
   switch ( op ) {
      case '+' : return 0; //加
      case '-' : return 1; //减
      case '*' : return 2; //乘
      case '^' : return 3; //乘方
      case '(' : return 4; //左括号
      case ')' : return 5;  //右括号
      case '\0': return 6; //起始符与终止符
   }
}

char priority ( char op1, char op2 ) //比较两个运算符之间的优先级
{ return pri[optr2rank ( op1 ) ][optr2rank ( op2 ) ]; }


int main(){
    ios::sync_with_stdio(false);
    Stack<Polynomial> opnd(500020);  //操作数栈
    Stack<char> optr(1000020);
    optr.push('\0');

    string cmd;
    cin>>cmd;
    if(cmd ==""){
        cout<<"0"<<endl;
        return 0;
    }
    string RealCmd = AddMultiple(cmd);  //真正的字符串（加完乘号的）
    RealCmd.push_back('\0');
    string::iterator it = RealCmd.begin();

    string toBeInt="";  //用于存储数字字符串
    while(!optr.empty() && it!=RealCmd.end()){
        if(IsDigit(*it)){   //如果是数字
            while(IsDigit(*it)){
                toBeInt.push_back(*it);
                it++;
            }
            opnd.Intpush(int(stoll(toBeInt)%M));    //将数字字符串转换为数字并入栈
            toBeInt = "";   //清空数字字符串
        }
        else if(*it=='x'){
            opnd.Xpush();   //x入栈
            it++;
        }
        else{
            switch(priority(optr.get_top(),*it)){   //比较优先级
                case '<':{  //栈顶优先级低
                    optr.push(*it); //当前运算符入栈
                    it++;   //指针后移
                    break;
                }
                case '=':{  //栈顶优先级等
                    optr.pop(); //弹出栈顶
                    it++;   //指针后移
                    break;
                }
                case '>':{  //栈顶优先级高
                    char op = optr.pop();   //弹出栈顶
                    opnd.calc(op);  //计算
                    break;
                }
            }
        }
    }
    Polynomial Final = opnd.pop();
    Final.print();
    return 0;
}