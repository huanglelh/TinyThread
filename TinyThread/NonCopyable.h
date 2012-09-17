#ifndef __NON_COPY_ABLE_H__
#define __NON_COPY_ABLE_H__
/**
*	\file NonCopyable.h
*   \brief ʹ���಻�ɿ�����
*   \remark �̳�CNonCopyable��������ɿ�����
*           ������ඨ����ʹ��DECLARE_NON_COPY_ABLE(ClassName)��;
*/
class CNonCopyable
{
protected:
	CNonCopyable(){}
	~CNonCopyable(){} //û���麯��������������Ϊ��

private:
	CNonCopyable(const CNonCopyable&);
	const CNonCopyable& operator = (const CNonCopyable&);
};

//ʹ��ע�����1��������ṩ�Լ��Ĺ��캯����2�����ඨ�����β��ʹ�á�
#define DECLARE_NON_COPY_ABLE(className) \
	private:\
		className(const className&);\
		const className& operator = (const className&);

#endif//__NON_COPY_ABLE_H__