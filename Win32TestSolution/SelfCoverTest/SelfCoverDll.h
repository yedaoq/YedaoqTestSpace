// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SELFCOVERDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SELFCOVERDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SELFCOVERDLL_EXPORTS
#define SELFCOVERDLL_API __declspec(dllexport)
#else
#define SELFCOVERDLL_API __declspec(dllimport)
#endif

extern SELFCOVERDLL_API HMODULE dll_handle;

SELFCOVERDLL_API int SelfCoverDll(void);
