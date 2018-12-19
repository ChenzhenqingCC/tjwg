
#pragma once

#define MAXLINKLEN 20			//链表的最大长度
typedef struct _NODE{
	int whotalk;
	char message[255];
	int color;
	_NODE *prev;			//指向链表的上一个节点
	_NODE *next;			//指向下一个节点
}NODE,*PNODE;
typedef struct _CLLIST{
	NODE *head;				//指向头节点的指针
	NODE *tail;				//指向尾节点的指针
	int len;				//链表包含的节点个数
}CLLIST;


//创建一个节点
NODE * CL_MakeNode(int whotalk,char *p,int color);
//释放一个节点
void CL_FreeNode(NODE *node);
//初始化链表
void CL_InitList(CLLIST &list);
//向链表头部添加一个节点
void CL_AddHead(CLLIST &list,NODE *node);
//向链表尾部添加一个节点
void CL_AddTail(CLLIST &list,NODE *node);
//从链表头部取一个节点
NODE * CL_GetHead(CLLIST &list);
//从链表尾部取一个节点
NODE * CL_GetTail(CLLIST &list);
//取链表的节点个数
int CL_GetLength(CLLIST &list);
//链表是否为空
BOOL CL_IsEmpty(CLLIST &list);
//链表是否已满
BOOL CL_IsFull(CLLIST &list);
//销毁链表
void CL_Destroy(CLLIST &list);

