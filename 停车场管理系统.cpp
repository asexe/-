#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cstring>
#define MAX 3 //车位数
#define OVERFLOW -2
#define Price 0.25 //每四分钟一块钱

typedef int Status;// 车辆信息
typedef struct {
	char num[10];  	// 车牌
	long reach;   	// 车辆到达时间
	long leave;   	// 车辆离开时间
	long fee;       //车辆所需缴纳费用
}Car;

typedef struct {
	Car* top;     //头指针
	Car* base;    //尾指针
	Car* content; //目标指针
	int stacksize;//栈可用的最大容量
}logStack;

// 车辆顺序栈：停车栈Enter和临时停车栈Temp
typedef struct {
	Car* top;     //头指针
	Car* base;    //尾指针
	int stacksize;//栈可用的最大容量
}SqStack;

//链栈（临时）
typedef struct StackNode {
	Car data;
	struct StackNode* next;
}StackNode,*LinkStack;

// 便道停车wait队列的结点
typedef struct QNode {
	Car data;
	QNode* next;
}QNode, * QueuePtr;

// 便道停车链式队列
typedef struct {
	QNode* front; //队头指针
    QNode* rear; //队尾指针
}LinkQueue;

//time以及相关函数组
long edge = 0;//edge为上一辆车的出场时间
long temp = 1;
long lleave;
//车辆随机进入时间
int intime(){
    temp = (rand() % 5*(1000-1))+ 1 + edge;//限制为5000-1的随机时间temp加上时间edge
    edge = temp + 1000;//temp值加上1000为车辆的入场时间
    return edge ;
}
//车辆随机离开时间，要求比上一辆入库车辆晚，比下一辆入库车辆时间早
int outtime(){
	lleave = edge + 25* ((rand() % (1000-1))+ 1);//出场时间为edge加上2500-1的随机时间
  return lleave;
}
//时间换算(传入单位为秒)
void Time(int time) {
    int min;
    int hour;
    int sec;
    if (time < 60) {
        printf("停车时长: %d 秒\n", time);
    }
    else if (time < 3600) {
        min = time / 60;
        sec = time % 60;
        if (time % 60 == 0) {
            printf("停车时长: %d 分钟\n", min);
        }
        else {
            printf("停车时长: %d 分钟 % d 秒\n", min , sec);
        }
    }
    else {
        min = time / 60;
        sec = time % 60;
        hour = min / 60;
        min %= 60;
        if (sec == 0 && min == 0) {
            printf("停车时长: %d 小时\n", hour);
        }
        else if(sec != 0 && min == 0){
            printf("停车时长: %d 小时 0 分钟 %d 秒\n", hour,sec);
        }
        else if (sec == 0 && min != 0) {
            printf("停车时长: %d 小时 %d 分钟\n", hour, min);
        }
        else if (sec != 0 && min != 0) {
            printf("停车时长: %d 小时 %d 分钟 %d 秒\n", hour,min, sec);
        }
    }
}
//传入的time单位为分钟
int price(int time) {
    int price;
    price = time * Price;
    return price;
}
// 车辆栈判空
int StackEmpty(SqStack S) {
    if (S.top == S.base) {
        printf("当前车库没有车辆！\n\a");
        return 0;
    }
    else {
        return 1;
    }
}
//查看车库
void SelectAll(SqStack S, LinkQueue Q) {
    if (StackEmpty(S)) {
        QueuePtr q=Q.front;
        printf("--------车库--------\n");
        while (S.top != S.base)
        {
            S.top--;
            printf("|%s|\n", S.top->num);
        }
        printf("--------便道---------\n");
        printf("-------------------------\n");
        if (q == Q.rear) {
            printf("暂无车辆等待...");
        }
        else {
            while (q != Q.rear) {
                q = q->next;
                printf("%s ",q->data.num);
            }
        }
        printf("\n-------------------------\n");
    }
}
// 初始化车辆栈
Status InitStack(SqStack& S) {
    S.base = new Car[MAX];
    if (!S.base)exit(OVERFLOW);
    S.top = S.base;
    S.stacksize = MAX;
    return 1;
}
//初始化临时栈
Status InitTempStack(LinkStack &TempStack) {
    TempStack = NULL;
    return 1;
}
//初始化日志栈
Status InitlogStack(logStack& L) {
	L.base = new Car[20];
	if (!L.base)exit(OVERFLOW);
	L.content = L.top = L.base;
	L.stacksize = 20;
	return 1;
}
// 车辆栈判满
int StackFull(SqStack S,Car e) {
    if (S.top - S.base == MAX) {
        printf("车位已满   车牌号 %s 进入便道排队！\a\n", e.num);
        return 0;
    }
    else {
        return 1;
    }
}
// 车辆进栈
Status Push(SqStack& S,logStack& L,Car e) {
    if (StackFull(S,e)) {
        *S.top++ = e;
		*L.top++= e;
		//printf("%ld\n",e.reach);
        printf("车牌号： %s 成功入库！\n",e.num);
        printf("当前车位剩余：%d \n", --S.stacksize);
        return 1;
    }
    //等待队列
    return 0;
}
// 取栈顶元素
Car GetTop(SqStack& S) {
    return *--S.top;
}
//判断有没有该车辆（栈）
Status Find1(SqStack S,Car e) {
    while (S.top != S.base)
    {
        S.top--;
        if (!strcmp(S.top->num, e.num)) {
            return 1;
        }
    }
    return 0;
}
//判断有没有该车辆（队列）
Status Find2(LinkQueue Q, Car e)
{
	QNode *p = new QNode;
	p = Q.front->next;
	while(p)
	{
		if(!strcmp(p->data.num, e.num))
			return 1;
		else
			p = p->next;
	}
	return 0;
}
// 链栈入栈（临时）
Status TempPush(LinkStack& TempStack,Car e) {
    LinkStack p = new StackNode;
    p->data = e;
    p->next = TempStack;
    TempStack = p;
    return 1;
}
// 链栈出栈（临时）
Car TempPop(LinkStack& TempStack) {
    LinkStack p;
    Car tempData;
    tempData = TempStack->data;
    p = TempStack;
    TempStack = TempStack->next;
    delete(p);
    return tempData;
}
// 车辆出栈
Status Pop(SqStack& S, Car& e, logStack& L,LinkStack& TempStack) {
    //是否空栈
		int pay = 0;
    if (StackEmpty(S)) {
        //判断有没有该车
        if (!Find1(S,e)) {
            printf("该车不在车库中！\a\n");
            return 0;
        }
        Car temp;
        while (true)
        {
            temp = GetTop(S);
            if (!strcmp(temp.num, e.num)) {
                printf("车牌号： %s 成功出库！\n", e.num);
                //时间(出库时间)
                e.leave = outtime();
                //计费(传入分钟单位)
                pay=price((e.leave - e.reach)/60);
                //出临时栈&进原栈(给车让位进另一个栈又出栈进回原栈)
                while (TempStack != NULL) {
                    *S.top++ = TempPop(TempStack);
                }
                //时间换算并输出时间
								L.content=L.top;
								//将出入时间写入记录栈
								while (L.content != L.base)
								{
									L.content--;
									if (!strcmp(L.content->num, e.num)) {
										if(L.content->fee ==0){
											L.content->fee = pay;
											L.content->leave = e.leave;
										}
									}
									if(L.content==L.base){
										break;
									}
								}
                Time(e.leave - e.reach);
                printf("请缴费： %d 元\n", pay);
                printf("车位剩余：%d \n", ++S.stacksize);
                return 1;
				    }
            //进栈(让位的车进临时栈)
            TempPush(TempStack, temp);
        }
    }
    return 0;
}
// 初始化车辆链式队列
Status InitQueue(LinkQueue& Q) {
    Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
    if (!Q.front)return 0;
    else {
        Q.front->next = NULL;
        return 1;
    }
}
// 车辆队列判空
int QueueEmpty(LinkQueue Q) {
    if (Q.front == Q.rear) {
        //printf("队空！\n\a");
        return 0;
    }return 1;
}
// 取链队的对头元素
Car GetHead(LinkQueue Q) {
    return Q.front->next->data;
}
// 车辆入队
Status EnQueue(LinkQueue& Q, Car e) {
    QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
    if (!p) {
        //printf("入队失败!\a\n");
        return 0;
    }
    else {
        p->data = e;
        p->next = NULL;
        Q.rear->next = p;
        Q.rear = p;
        return 1;
    }
}
// 车辆出队
Car DeQueue(LinkQueue& Q) {
    QueueEmpty(Q);
    Car HeadCar=GetHead(Q);
    QueuePtr p;
    p = Q.front->next;
    Q.front->next = p->next;
    if (Q.rear == p)Q.rear = Q.front;
    free(p);
    return HeadCar;
}
//打印停车场日志
void printlog(logStack L){
	printf("          车牌号");
	printf("           入场时间");
	printf("           出场时间");
	printf("           应缴费用\n");
	 while (L.top != L.base){
		   L.top--;
			 printf("%16s ",L.top->num );
			 printf("%16ld s ",L.top->reach);
			if(L.top->leave==0){
				printf("            未出场");
			}
			 else{
				 printf("%16ld s",L.top->leave);
			 }
			 printf("%16ld 元\n",L.top->fee);
		 }
}
int main() {
	SqStack S;
	logStack L;
	LinkStack TempStack;
	LinkQueue Q;
	Car NextCar;
	Car e;
    char item;
    srand((int)time(NULL));
	//栈初始化
	if (!InitStack(S)||!InitlogStack(L)) {
		printf("栈初始化失败！\n\a");
		return 0;
		system("pause");
	}
	//临时栈初始化
	InitTempStack(TempStack);
	//队列初始化
	if (!InitQueue(Q)) {
		printf("队列初始化失败！\n\a");
		return 0;
		system("pause");
	}
	while (1) {
		printf("***欢迎使用停车场管理系统***\n");
		printf("***本车场每四分钟 一 块钱***\n");
		printf("1.车辆驶入\n");
		printf("2.车辆驶出\n");
        printf("3.打印日志\n");
		printf("4.退出\n");
		printf("请输入相应数字选择功能: ");
		scanf("%s",&item);
		printf("-----------------\n");
		if(strlen(&item)!=1){
            printf("错误：输入正确的功能序号！\a\n\n\n");
            SelectAll(S, Q);
        }
		else
		{switch (item)
		{
		case '1':{
			printf("车牌号为：");
			scanf("%s", e.num);
            if(Find1(S,e)){
                printf("此车已存在于停车场!请重新输入！\n\n");
                break;
            }
            if(Find2(Q,e)){
                printf("此车已存在于便道!请重新输入！\n\n");
                break;
            }
			//时间计费(入库开始时间)，记录车辆车牌和进入时间
			if (StackFull(S,e)) {
				e.reach=intime();
        e.leave = 0;
        e.fee = 0;
				Push(S,L,e);
			}
				else
				EnQueue(Q,e);
			  printf("\n\n");
        SelectAll(S, Q);
			break;
		}
		case '2':{
			printf("输入要驶出的车辆：");
			scanf("%s", e.num);
			if (Pop(S, e, L,TempStack)) {
				if (QueueEmpty(Q)) {
					//出队进栈
					NextCar = DeQueue(Q);
					NextCar.reach = intime();
					Push(S,L,NextCar);
				}
			}
			printf("\n\n");
      SelectAll(S, Q);
			break;
		}
    case '3':{
			printlog(L);
			SelectAll(S, Q);
			break;
    }
		case '4':{
			return 0;
			system("pause");
		}
		default:printf("错误：输入正确的功能序号！\a\n\n\n");
		    SelectAll(S, Q);
			break;
		}
	}
	//system("pause");
	}
	return 0;
}
