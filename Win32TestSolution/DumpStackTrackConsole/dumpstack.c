#include <stdio.h>

/* ��ӡ����ջ�������� */
#define DUMP_STACK_DEPTH_MAX 16

// /* ��ȡ�Ĵ���ebp��ֵ */
// void get_ebp(unsigned long *ebp) {
//     __asm__ __volatile__ (
//         "mov %%ebp, %0"
//         :"=m"(*ebp)
//         ::"memory");
// }

/* ��ȡ����ջ */
int __cdecl dump_stack(unsigned long ebp, void **stack, int size) {
    int depth = 0;

    /* 2.������ջ��ַ */
    for (depth = 0; (depth < size) && (0 != ebp) && (0 != *(unsigned long *)ebp) && (ebp != *(unsigned long *)ebp); ++depth) {
        stack[depth] = (void *)(*(unsigned long *)(ebp + sizeof(unsigned long)));
        ebp = *(unsigned long *)ebp;
    }

    return depth;
}

// int dump_stack_current(void **stack, int size)
// {
// 	unsigned long ebp = 0;
// 	/* 1.�õ��ײ㺯����ջ��ַ */
// 	get_ebp(&ebp);
// 
// 	return dump_stack(ebp, stack, size);
// }

// /* ���Ժ��� 2 */
// void test_meloner() {
//     void *stack[DUMP_STACK_DEPTH_MAX] = {0};
//     int stack_depth = 0;
//     int i = 0;
// 
//     /* ��ȡ����ջ */
//     stack_depth = dump_stack(stack, DUMP_STACK_DEPTH_MAX);
// 
//     /* ��ӡ����ջ */
//     printf(" Stack Track: \r\n");
//     for (i = 0; i < stack_depth; ++i) {
//         printf(" [%d] %p \r\n", i, stack[i]);
//     }
// 
//     return;
// }
// 
// /* ���Ժ��� 1 */
// void test_hutaow() {
//     test_meloner();
//     return;
// }
// 
// /* ������ */
// int main(int argc, char *argv[]) {
//     test_hutaow();
//     return 0;
// }

