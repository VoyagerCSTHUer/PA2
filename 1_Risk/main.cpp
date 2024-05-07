#include <iostream>
using namespace std;

class FakeQueue{
private:
    int* data;
    int first;
    int last;
    int capacity;
    int status;
public:
    FakeQueue(int size){
        data = new int[size];
        first = 0;
        last = 0;
        capacity = size;
    }

    bool isEmpty(){
        return first == last;
    }

    bool isFull(){
        return last==capacity;
    }

    void enqueue(int value){
        if(isFull()){
            //cout<<"Queue is full."<<endl;
        }else{
            data[last] = value;
            last += 1;
        }
    }

    void dequeue(){
        if(isEmpty()){
            //cout<<"Queue is empty."<<endl;
        }else{
            first += 1;
        }
    }

    void update(int value){
        if(isEmpty())return;
        for(int i=first;i<last;i++){
            data[i] = max(data[i],value);
        }
    }

    int Emptize(){
        int temp = last - first;
        first = last;
        return temp;
    }

    void printLow(){
        int count = 0;
        for(int i=0;i<last;i++){
            if(data[i]==0)count++;
        }
        printf("%d ",count);
    }

    void printMid(){
        int count = 0;
        for(int i=0;i<last;i++){
            if(data[i]==1)count++;
        }
        printf("%d\n",count);
    }

    void init(){
        first = 0;
        last = 0;
    }

    void print(){
        for(int i=0;i<last;i++){
            cout<<data[i]<<" ";
        }
        cout<<endl;
    
    }
};


int RiskLevel(int k,int p,int q){
    if(k<p)return 0;
    else if(k<q)return 1;
    return 2;
}

int main(){
    ios::sync_with_stdio(false);
    int n; //总天数
    cin>>n;
    int* patient = new int[n+1]; //患者人数
    int* start = new int[n+1];  //开始追踪的天数
    int T;  //p,q的组数
    int p,q;
    FakeQueue Q(n+1);

    //输入
    patient[0] = 0;
    start[0] = 0;
    for(int i=1;i<=n;i++){
        cin>>patient[i];
    }
    int temp;
    for(int j=1;j<=n;j++){
        cin>>temp;
        start[j] = j-temp;
    }
    cin>>T;

    for(int t=0;t<T;t++){
        cin>>p>>q;
        int i=0;
        int j=1;

        while(i<=n){  
            //ans[i] = status;
            /*if(RiskLevel(patient[i],p,q)==2){
                i+=Q.Emptize();
                continue;
            }*/
            Q.dequeue();
            Q.update(RiskLevel(patient[i],p,q));
            while(start[j]<=i){
                Q.enqueue(0);
                j++;
            }
            i++;
        }
        Q.printLow();
        Q.printMid();
        Q.init();
    }

}