#ifndef __REFERENCE_COUNTER_H__
#define __REFERENCE_COUNTER_H__
#ifdef _WIN32
/**
*	\file RefCounter.h
*	\brief  �ṩ���ü�����������Ҫʹ�����ü�������ֱ�Ӽ̳б��࣬���ɾ߱����ü������� 
*			��ɾۺϱ���Ķ���ﵽ���ü�����Ŀ�ģ��Ƽ�ʹ�ü̳�
*			ʹ��CSmartPtr���Դﵽ�Զ������ü������й����Ч��
*			ʹ�÷���:1���¶�����ࣨCNew���̳���CRefCounter����������ӵ�����ü������ܡ�
*			2�����������ࣺtypedef CSmartPtr<CNew> CNewPtr; 
*			3�����ˣ��Ϳ�����ʹ��CNew��ָ���ˣ��磺CNewPtr newPtr = new CNew;
*	\remark ���ü���ʹ��ע�⣺
*			���ü���ʹ��ʱ�����ܳ���ѭ�����õ�������磺 A����B�� B������A����
*			�����������ʱ˫�����ͷŲ��ˣ�������������״������ʹ�����ü����Ķ���ϵͳ
*			Ӧ����������õ����νṹ�����Խṹ������Ӧ�ó��ֱջ���״����������Ƶ�ְ��
*/
#include <Windows.h>

namespace Utility
{
	class CRefCounter
	{
	public:
		CRefCounter();
		/**
		*	\brief �������캯�������������ü��������ü������㿪ʼ
		*/
		CRefCounter(const CRefCounter&);
		virtual ~CRefCounter()
		{
		}
		/**
		*	\brief ��ֵ�������ı����ü���
		*/
		CRefCounter& operator= (const CRefCounter&)
		{
			return *this;
		}

		/**
		*	\brief ��������
		*/
		virtual void addRef();
		/**
		*	\brief ��������
		*/
		virtual void release();
		/**
		*	\brief ��õ�ǰ��������
		*/
		virtual LONG getRefCnt() const ;
		/**
		*	\brief ���õ����ü������ٵ�0ʱ�Ĳ�����ʽ��
		*		false��ʾdelete���Լ���true��ʾ��delete
		*		������ʱĬ��Ϊfalse,���������ٵ�0ʱdelete�Լ�
		*/
		virtual void setNoDelete(bool);

	protected:
		volatile LONG m_lRef;
		bool m_bNoDelete;

	};

	/**
	*	\brief �κμ̳���CRefCounter���࣬����ʹ�ø���ﵽ����ָ���Ч��
	*/
	template<typename T>
	class CSmartPtr
	{
	public:
		CSmartPtr(T* p = NULL)
		{
			m_p = p;
			if ( m_p )
			{
				m_p->addRef();	
			}
		}
		CSmartPtr(const CSmartPtr<T>& r)
		{
			m_p = r.m_p;
			if ( m_p )
			{
				m_p->addRef();
			}
		}
		~CSmartPtr()
		{
			if ( m_p )
			{
				m_p->release();
			}
		}
		CSmartPtr& operator = (const CSmartPtr<T> &r)
		{
			if ( r.m_p != m_p )
			{
				if ( m_p )
				{
					m_p->release();
				}

				m_p = r.m_p;
				if ( m_p )
				{
					m_p->addRef();
				}
				
			}
			return *this;
		}
		CSmartPtr& operator = (T* p)
		{
			if ( m_p != p )
			{
				if ( m_p )
				{
					m_p->release();
				}

				if ( p )
				{
					p->addRef();
				}
				m_p = p;
			}
			return *this;
		}
		
		T* get() const
		{
			return m_p;
		}

		/**
		*	\brief ע�⣺�ú����������쳣����ָ��Ϊ��ʱ���쳣
		*			��ʱ�Ҳ����ڴ˴������쳣�ķ�����
		*/
		T* operator ->() const
		{
			assert( m_p != NULL );
			if ( !m_p )
			{
				throwNullException();
			}
			return m_p;
			
		}

		/**
		*	\brief ע�⣺�ú����������쳣����ָ��Ϊ��ʱ���쳣
		*			��ʱ�Ҳ����ڴ˴������쳣�ķ�����
		*/
		T& operator *() const
		{
			assert( m_p != NULL );

			if ( !m_p )
			{
				throwNullException();
			}
			return *(m_p);
		}
		/**
		*	\brief �ﵽʹ��if(x)�ж�ָ���Ƿ�Ϊ�յ�Ч����xΪCSmarPtr����
		*/
		operator bool() const
		{
			return m_p ? true : false;
		}
		
		/**
		*	\brief ʵ�ֶ�̬����ת��
		*/
		template<typename Y>
		static CSmartPtr dynamicCast( const CSmartPtr<Y> &r)
		{
			return CSmartPtr<T>(dynamic_cast<T*>(r.m_p));
		}
		
		/**
		*	\brief ʵ�ֽ�ͬһ�̳���ϵ��ָ������ת���ɸ�����ָ�����
		*/
		template<typename Y>
		static CSmartPtr dynamicCast( Y* p )
		{
			return CSmartPtr<T>(dynamic_cast<T*>(p));
		}

	protected:
		void throwNullException() const 
		{
			std::string strErrMsg =  "SmartPtr using null pointer: ";
			strErrMsg += __FILE__;
			throw  strErrMsg;
		}
	public:
		T* m_p;
	};
}

#endif //_WIN32

#endif//!__REFERENCE_COUNTER_H__