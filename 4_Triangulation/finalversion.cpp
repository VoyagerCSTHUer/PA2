#include <iostream>
using namespace std;

//栈
template <typename T>
class Stack{
private:
    T* data;
    int top;
    int capacity;
public:
    Stack(int size){
        data = new T[size]();
        top = 0;
        capacity = size;
    }
    T pop(){
        if(IsEmpty());
        return data[--top];
    }
    void push(T x){
        data[top++] = x;
    }
    T& get_top(){
        if(IsEmpty());
        return data[top-1];
    }
    T& get_second_top(){
        if(get_size()<2);
        return data[top-2];
    }
    bool IsEmpty(){
        return top==0;
    }
    bool IsFull(){
        return top==capacity;
    }
    int get_size(){
        return top;
    }
    void printStack(){
        for(int i=0;i<top;i++){
            cout<<data[i]<<' ';
        }
        cout<<endl;
    }
};

//队列
template <typename T>
class Queue{
private:
    T* data;
    int front;
    int back;
    int capacity;
public:
    Queue(int size){
        data = new T[size]();
        front = 0;
        back = 0;
        capacity = size;
    }
    T dequeue(){
        if(IsEmpty());
        return data[front++];
    }
    void enqueue(T x){
        data[back++] = x;
    }
    T& get_front(){
        if(IsEmpty());
        return data[front];
    }
    T& get_back(){
        if(IsEmpty());
        return data[back-1];
    }
    T& get_back_of_back(){
        if(get_size()<2);
        return data[back-2];
    }
    T pop_back(){
        if(IsEmpty());
        return data[--back];
    }
    bool IsEmpty(){
        return front==back;
    }
    bool IsFull(){
        return back==capacity;
    }
    int get_size(){
        return (back-front);
    }
    void printQueue(){
        for(int i=front;i<back;i++){
            cout<<data[i]<<' ';
        }
        cout<<endl;
    }
};

//点对，包含坐标和“是否是位于上方的点”
struct MyPair{
    int x;
    int y;
    bool IsTop;
    MyPair():x(0),y(0),IsTop(true){}
    friend ostream& operator<<(ostream& os, const MyPair& pair) {
        os << pair.x << ' ' << pair.y << ' ';
        return os;
    }
};

//三元组，包含三个点对（实际是六个数）
struct Tuple{
    int xa;
    int xb;
    int ya;
    int yb;
    int za;
    int zb;
    Tuple(MyPair a,MyPair b,MyPair c):xa(a.x),xb(a.y),ya(b.x),yb(b.y),za(c.x),zb(c.y){} //从三个MyPair转化而来的构造函数
    Tuple(){}
};

//判断是否是优节点（与点在上方和下方有关）
bool GoodPoint(MyPair & a,MyPair & b, MyPair & c){
    bool counterclockwise;
    long long int v1_x = a.x - b.x;
    long long int v1_y = a.y - b.y;
    long long int v2_x = c.x - b.x;
    long long int v2_y = c.y - b.y;
    if(v1_x*v2_y >= v1_y*v2_x)counterclockwise = false; //v1在v2的顺时针方向
    else counterclockwise = true;
    if((counterclockwise && c.IsTop) || (!counterclockwise && !c.IsTop))return true;
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    int n;
    cin>>n;
    MyPair* points = new MyPair[n]();   //存储所有点对
    int minindex = 0;   //最左侧的点的下标
    bool assumeConterClockwise = true;  //假设是逆时针方向
    for(int i=0;i<n;i++){
        cin>>points[i].x>>points[i].y;
    }

    //找到最左侧的点以及判断顺逆时针，在模n意义下寻找
    for(int i=0;i<n;i++){
        if(points[i].x>points[(i-1+n)%n].x && points[(i-1+n)%n].x < points[(i-2+n)%n].x){
            minindex = (i-1+n)%n;
            if(points[i].y < points[(i-2+n)%n].y)assumeConterClockwise=true;
            else assumeConterClockwise=false;
            break;
        }
    }

    //保留原有结构，更改为从左到右的顺时针点列（comfortablePoints）
    Queue<MyPair> comfortablePoints(n);
    if(!assumeConterClockwise){
        for(int i=0;i<n;i++){
            comfortablePoints.enqueue(points[(minindex+i)%n]);
        }
    }
    else{
        for(int i=0;i<n;i++){
            comfortablePoints.enqueue(points[(minindex-i+n)%n]);
        }
    }

    Queue<MyPair> top(n+2); //上方的点
    Stack<MyPair> bottom(n+2);  //下方的点
    Stack<MyPair> operable(n+2);    //操作栈
    MyPair startSentinel;   //起始哨兵
    MyPair endSentinel; //终止哨兵
    MyPair temp;    //临时变量，防止重复声明
    startSentinel = comfortablePoints.dequeue();    

    top.enqueue(startSentinel); //起始哨兵入队，防止188行get_back()出错

    //将所有点分为上方和下方
    while(!comfortablePoints.IsEmpty()){
        temp = comfortablePoints.dequeue();
        if(temp.x > top.get_back().x){
            temp.IsTop = true;
            top.enqueue(temp);
        }//上方点入队
        else{
            temp.IsTop = false;
            bottom.push(temp);
            break;
        }//下方点入栈
    }
    top.dequeue();  //起始哨兵出队
    endSentinel = top.pop_back();   //终止哨兵出队
    int rest = n - 3 - top.get_size();  //剩余点数
    //将剩余点进入下方栈
    while(rest>0){
        temp = comfortablePoints.dequeue();
        temp.IsTop = false;
        bottom.push(temp);
        rest--;
    }
    

    Queue<MyPair> thequeue(n);  //整合后的队列
    Queue<Tuple> ans(n);    //答案队列

    //输入整合为thequeue
    if(!top.IsEmpty() && !bottom.IsEmpty()){
        //当两个队列都不为空时
        while(!top.IsEmpty() && !bottom.IsEmpty()){
            if(top.get_front().x > bottom.get_top().x){
                thequeue.enqueue(bottom.pop());
            }
            else{
                thequeue.enqueue(top.dequeue());
            }
        }
    }

    //此处一定有至少一个栈或队列为空，将另一方清空
    if(top.IsEmpty()){
        while(!bottom.IsEmpty()){
            thequeue.enqueue(bottom.pop());
        }
    }
    else if(bottom.IsEmpty()){
        while(!top.IsEmpty()){
            thequeue.enqueue(top.dequeue());
        }
    }
    
    //开始扫描
    operable.push(startSentinel);   //起始哨兵入栈
    operable.push(thequeue.dequeue());  //第一个点入栈，此后栈中至少有两个元素
    bool operableIsTop = operable.get_top().IsTop;  //操作栈属性（优链属于上方还是下方）
    MyPair tmp;
    MyPair poped;

    //扫描过程
    while(!thequeue.IsEmpty()){
        tmp = thequeue.dequeue();   //取出下一个点
        //case_1
        if(tmp.IsTop!=operableIsTop){   //优链属性不同
            MyPair formerTop; 
            formerTop = operable.get_top();   //记录栈顶元素
            //清空栈并存储答案
            while(operable.get_size()>=2){
                poped = operable.pop();
                ans.enqueue(Tuple(poped,operable.get_top(),tmp));
            }
            operable.pop(); //弹出剩下的一个元素
            operable.push(formerTop);   //栈顶元素重新入栈
            operable.push(tmp); //新元素入栈
            operableIsTop = tmp.IsTop;  //更新操作栈属性
        }
        //case_2
        else{
            //case_2a
            if(GoodPoint(operable.get_second_top(),operable.get_top(),tmp))operable.push(tmp);  //优节点直接入栈
            //case_2b
            else{
                //弹出劣节点并存储答案
                while(operable.get_size()>=2 && !GoodPoint(operable.get_second_top(),operable.get_top(),tmp)){
                    poped = operable.pop();
                    ans.enqueue(Tuple(poped,operable.get_top(),tmp));
                }
                operable.push(tmp); //劣节点入栈
            }
        }
    }

    //最后的处理（尾哨兵收场）
    while(operable.get_size()>=2){
        poped = operable.pop();
        ans.enqueue(Tuple(poped,operable.get_top(),endSentinel));
    }

    //输出答案
    Tuple Temp;
    while(!ans.IsEmpty()){
        Temp = ans.dequeue();
        printf("%d %d %d %d %d %d \n",Temp.xa,Temp.xb,Temp.ya,Temp.yb,Temp.za,Temp.zb);
    }
}