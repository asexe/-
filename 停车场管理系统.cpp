#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cstring>
#define MAX 3 //��λ��
#define OVERFLOW -2
#define Price 0.25 //ÿ�ķ���һ��Ǯ

typedef int Status;// ������Ϣ
typedef struct {
	char num[10];  	// ����
	long reach;   	// ��������ʱ��
	long leave;   	// �����뿪ʱ��
	long fee;       //����������ɷ���
}Car;

typedef struct {
	Car* top;     //ͷָ��
	Car* base;    //βָ��
	Car* content; //Ŀ��ָ��
	int stacksize;//ջ���õ��������
}logStack;

// ����˳��ջ��ͣ��ջEnter����ʱͣ��ջTemp
typedef struct {
	Car* top;     //ͷָ��
	Car* base;    //βָ��
	int stacksize;//ջ���õ��������
}SqStack;

//��ջ����ʱ��
typedef struct StackNode {
	Car data;
	struct StackNode* next;
}StackNode,*LinkStack;

// ���ͣ��wait���еĽ��
typedef struct QNode {
	Car data;
	QNode* next;
}QNode, * QueuePtr;

// ���ͣ����ʽ����
typedef struct {
	QNode* front; //��ͷָ��
    QNode* rear; //��βָ��
}LinkQueue;

//time�Լ���غ�����
long edge = 0;//edgeΪ��һ�����ĳ���ʱ��
long temp = 1;
long lleave;
//�����������ʱ��
int intime(){
    temp = (rand() % 5*(1000-1))+ 1 + edge;//����Ϊ5000-1�����ʱ��temp����ʱ��edge
    edge = temp + 1000;//tempֵ����1000Ϊ�������볡ʱ��
    return edge ;
}
//��������뿪ʱ�䣬Ҫ�����һ����⳵��������һ����⳵��ʱ����
int outtime(){
	lleave = edge + 25* ((rand() % (1000-1))+ 1);//����ʱ��Ϊedge����2500-1�����ʱ��
  return lleave;
}
//ʱ�任��(���뵥λΪ��)
void Time(int time) {
    int min;
    int hour;
    int sec;
    if (time < 60) {
        printf("ͣ��ʱ��: %d ��\n", time);
    }
    else if (time < 3600) {
        min = time / 60;
        sec = time % 60;
        if (time % 60 == 0) {
            printf("ͣ��ʱ��: %d ����\n", min);
        }
        else {
            printf("ͣ��ʱ��: %d ���� % d ��\n", min , sec);
        }
    }
    else {
        min = time / 60;
        sec = time % 60;
        hour = min / 60;
        min %= 60;
        if (sec == 0 && min == 0) {
            printf("ͣ��ʱ��: %d Сʱ\n", hour);
        }
        else if(sec != 0 && min == 0){
            printf("ͣ��ʱ��: %d Сʱ 0 ���� %d ��\n", hour,sec);
        }
        else if (sec == 0 && min != 0) {
            printf("ͣ��ʱ��: %d Сʱ %d ����\n", hour, min);
        }
        else if (sec != 0 && min != 0) {
            printf("ͣ��ʱ��: %d Сʱ %d ���� %d ��\n", hour,min, sec);
        }
    }
}
//�����time��λΪ����
int price(int time) {
    int price;
    price = time * Price;
    return price;
}
// ����ջ�п�
int StackEmpty(SqStack S) {
    if (S.top == S.base) {
        printf("��ǰ����û�г�����\n\a");
        return 0;
    }
    else {
        return 1;
    }
}
//�鿴����
void SelectAll(SqStack S, LinkQueue Q) {
    if (StackEmpty(S)) {
        QueuePtr q=Q.front;
        printf("--------����--------\n");
        while (S.top != S.base)
        {
            S.top--;
            printf("|%s|\n", S.top->num);
        }
        printf("--------���---------\n");
        printf("-------------------------\n");
        if (q == Q.rear) {
            printf("���޳����ȴ�...");
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
// ��ʼ������ջ
Status InitStack(SqStack& S) {
    S.base = new Car[MAX];
    if (!S.base)exit(OVERFLOW);
    S.top = S.base;
    S.stacksize = MAX;
    return 1;
}
//��ʼ����ʱջ
Status InitTempStack(LinkStack &TempStack) {
    TempStack = NULL;
    return 1;
}
//��ʼ����־ջ
Status InitlogStack(logStack& L) {
	L.base = new Car[20];
	if (!L.base)exit(OVERFLOW);
	L.content = L.top = L.base;
	L.stacksize = 20;
	return 1;
}
// ����ջ����
int StackFull(SqStack S,Car e) {
    if (S.top - S.base == MAX) {
        printf("��λ����   ���ƺ� %s �������Ŷӣ�\a\n", e.num);
        return 0;
    }
    else {
        return 1;
    }
}
// ������ջ
Status Push(SqStack& S,logStack& L,Car e) {
    if (StackFull(S,e)) {
        *S.top++ = e;
		*L.top++= e;
		//printf("%ld\n",e.reach);
        printf("���ƺţ� %s �ɹ���⣡\n",e.num);
        printf("��ǰ��λʣ�ࣺ%d \n", --S.stacksize);
        return 1;
    }
    //�ȴ�����
    return 0;
}
// ȡջ��Ԫ��
Car GetTop(SqStack& S) {
    return *--S.top;
}
//�ж���û�иó�����ջ��
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
//�ж���û�иó��������У�
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
// ��ջ��ջ����ʱ��
Status TempPush(LinkStack& TempStack,Car e) {
    LinkStack p = new StackNode;
    p->data = e;
    p->next = TempStack;
    TempStack = p;
    return 1;
}
// ��ջ��ջ����ʱ��
Car TempPop(LinkStack& TempStack) {
    LinkStack p;
    Car tempData;
    tempData = TempStack->data;
    p = TempStack;
    TempStack = TempStack->next;
    delete(p);
    return tempData;
}
// ������ջ
Status Pop(SqStack& S, Car& e, logStack& L,LinkStack& TempStack) {
    //�Ƿ��ջ
		int pay = 0;
    if (StackEmpty(S)) {
        //�ж���û�иó�
        if (!Find1(S,e)) {
            printf("�ó����ڳ����У�\a\n");
            return 0;
        }
        Car temp;
        while (true)
        {
            temp = GetTop(S);
            if (!strcmp(temp.num, e.num)) {
                printf("���ƺţ� %s �ɹ����⣡\n", e.num);
                //ʱ��(����ʱ��)
                e.leave = outtime();
                //�Ʒ�(������ӵ�λ)
                pay=price((e.leave - e.reach)/60);
                //����ʱջ&��ԭջ(������λ����һ��ջ�ֳ�ջ����ԭջ)
                while (TempStack != NULL) {
                    *S.top++ = TempPop(TempStack);
                }
                //ʱ�任�㲢���ʱ��
								L.content=L.top;
								//������ʱ��д���¼ջ
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
                printf("��ɷѣ� %d Ԫ\n", pay);
                printf("��λʣ�ࣺ%d \n", ++S.stacksize);
                return 1;
				    }
            //��ջ(��λ�ĳ�����ʱջ)
            TempPush(TempStack, temp);
        }
    }
    return 0;
}
// ��ʼ��������ʽ����
Status InitQueue(LinkQueue& Q) {
    Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
    if (!Q.front)return 0;
    else {
        Q.front->next = NULL;
        return 1;
    }
}
// ���������п�
int QueueEmpty(LinkQueue Q) {
    if (Q.front == Q.rear) {
        //printf("�ӿգ�\n\a");
        return 0;
    }return 1;
}
// ȡ���ӵĶ�ͷԪ��
Car GetHead(LinkQueue Q) {
    return Q.front->next->data;
}
// �������
Status EnQueue(LinkQueue& Q, Car e) {
    QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
    if (!p) {
        //printf("���ʧ��!\a\n");
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
// ��������
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
//��ӡͣ������־
void printlog(logStack L){
	printf("          ���ƺ�");
	printf("           �볡ʱ��");
	printf("           ����ʱ��");
	printf("           Ӧ�ɷ���\n");
	 while (L.top != L.base){
		   L.top--;
			 printf("%16s ",L.top->num );
			 printf("%16ld s ",L.top->reach);
			if(L.top->leave==0){
				printf("            δ����");
			}
			 else{
				 printf("%16ld s",L.top->leave);
			 }
			 printf("%16ld Ԫ\n",L.top->fee);
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
	//ջ��ʼ��
	if (!InitStack(S)||!InitlogStack(L)) {
		printf("ջ��ʼ��ʧ�ܣ�\n\a");
		return 0;
		system("pause");
	}
	//��ʱջ��ʼ��
	InitTempStack(TempStack);
	//���г�ʼ��
	if (!InitQueue(Q)) {
		printf("���г�ʼ��ʧ�ܣ�\n\a");
		return 0;
		system("pause");
	}
	while (1) {
		printf("***��ӭʹ��ͣ��������ϵͳ***\n");
		printf("***������ÿ�ķ��� һ ��Ǯ***\n");
		printf("1.����ʻ��\n");
		printf("2.����ʻ��\n");
        printf("3.��ӡ��־\n");
		printf("4.�˳�\n");
		printf("��������Ӧ����ѡ����: ");
		scanf("%s",&item);
		printf("-----------------\n");
		if(strlen(&item)!=1){
            printf("����������ȷ�Ĺ�����ţ�\a\n\n\n");
            SelectAll(S, Q);
        }
		else
		{switch (item)
		{
		case '1':{
			printf("���ƺ�Ϊ��");
			scanf("%s", e.num);
            if(Find1(S,e)){
                printf("�˳��Ѵ�����ͣ����!���������룡\n\n");
                break;
            }
            if(Find2(Q,e)){
                printf("�˳��Ѵ����ڱ��!���������룡\n\n");
                break;
            }
			//ʱ��Ʒ�(��⿪ʼʱ��)����¼�������ƺͽ���ʱ��
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
			printf("����Ҫʻ���ĳ�����");
			scanf("%s", e.num);
			if (Pop(S, e, L,TempStack)) {
				if (QueueEmpty(Q)) {
					//���ӽ�ջ
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
		default:printf("����������ȷ�Ĺ�����ţ�\a\n\n\n");
		    SelectAll(S, Q);
			break;
		}
	}
	//system("pause");
	}
	return 0;
}
