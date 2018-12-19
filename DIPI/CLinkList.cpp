#include "stdafx.h"
#include "CLinkList.h"
//创建一个节点
NODE * CL_MakeNode(int whotalk,char *p,int color)
{
	NODE *node=new NODE;
	node->whotalk=whotalk;
	strcpy_s(node->message,p);
	node->color=color;
	node->prev=NULL;
	node->next=NULL;
	return node;
}
//释放一个节点
void CL_FreeNode(NODE *node)
{
	if(node!=NULL)
		delete node;
}
//初始化链表
void CL_InitList(CLLIST &list)
{
	list.head=NULL;
	list.tail=NULL;
	list.len=0;
}
//向链表头部添加一个节点
void CL_AddHead(CLLIST &list,NODE *node)
{
	if(list.len<MAXLINKLEN){
		if(list.len>0){
			NODE *p=list.head;
			list.head=node;
			node->prev=NULL;
			node->next=p;
			p->prev=node;
		}
		else{
			list.head=node;
			list.tail=node;
			node->prev=NULL;
			node->next=NULL;
		}
		list.len++;
	}
}
//向链表尾部添加一个节点
void CL_AddTail(CLLIST &list,NODE *node)
{
	if(list.len<MAXLINKLEN){
		if(list.len>0){
			NODE *p=list.tail;
			list.tail=node;
			node->next=NULL;
			node->prev=p;
			p->next=node;
		}
		else{
			list.head=node;
			list.tail=node;
			node->prev=NULL;
			node->next=NULL;
		}
		list.len++;
	}
}
//从链表头部取一个节点
NODE * CL_GetHead(CLLIST &list)
{
	NODE *p=NULL;
	if(list.len>0){
		p=list.head;
		if(p->next!=NULL)
			list.head=p->next;
		else{
			list.head=NULL;
			list.tail=NULL;
		}
		list.len--;
	}
	return p;
}
//从链表尾部取一个节点
NODE * CL_GetTail(CLLIST &list)
{
	NODE *p=NULL;
	if(list.len>0){
		p=list.tail;
		if(p->prev!=NULL)
			list.tail=p->prev;
		else{
			list.head=NULL;
			list.tail=NULL;
		}
		list.len--;
	}
	return p;
}
//取链表的节点个数
int CL_GetLength(CLLIST &list)
{
	return list.len;
}
//链表是否为空
BOOL CL_IsEmpty(CLLIST &list)
{
	if(list.len==0)
		return TRUE;
	else
		return FALSE;
}
//链表是否已满
BOOL CL_IsFull(CLLIST &list)
{
	if(list.len==MAXLINKLEN)
		return TRUE;
	else
		return FALSE;
}
//销毁链表
void CL_Destroy(CLLIST &list)
{
	int i;
	NODE *p;
	for(i=0;i<list.len;i++){
		p=CL_GetHead(list);
		CL_FreeNode(p);
	}
	CL_InitList(list);
}

