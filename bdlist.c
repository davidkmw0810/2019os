#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

static LIST_HEAD(birthday_list);

struct birthday *createBirthday(int day, int month, int year) {

	struct birthday *days = kmalloc(sizeof(struct birthday), GFP_KERNEL);

	days->day = day;
	days->month = month;
	days->year = year;
	INIT_LIST_HEAD(&days->list);
	list_add_tail(&days->list, &birthday_list);

	return days;
	/* TODO: 생일을 위한 메모리를 할당하고, 인자들을 채워 생일을 완성하세요. */

}

int simple_init(void) {

	INIT_LIST_HEAD(&birthday_list);
	createBirthday(10, 8, 1996);
	createBirthday(19, 3, 2004);
	createBirthday(23, 5, 1997);
	createBirthday(13, 12, 2000);
	createBirthday(7, 9, 1980);
	/* TODO: 생일 목록을 하나씩 생성하는대로 연결리스트에 연결시키세요(노드 삽입). */

	struct birthday *cursor;
	list_for_each_entry(cursor, &birthday_list, list){
		
		printk("DAY %d.%d.%d", cursor->day, cursor->month, cursor->year);
	}
	/* TODO: 완성된 연결리스트를 탐색하는 커널 함수를 사용하여 출력하세요. */

  return 0;
}

void simple_exit(void) {
	
	/* 모듈을 제거할 때는 생성한 연결 리스트도 하나씩 제거하며 끝내도록 하세요. */
	/* 제거를 하기 전에 리스트가 "비어있을 경우""에 대한 예외처리를 하는게 좋겠죠? */
	if(list_empty(&birthday_list)) {
		printk("List is Empty\n");
	    return;
	}
	
	struct birthday *data;
	struct list_head *ptr, *next;
	list_for_each_safe(ptr, next, &birthday_list){	
				
		
		data = list_entry(ptr, struct birthday, list);
		printk("REMOVING %d.%d.%d", data->day, data->month, data->year);

		list_del(ptr);
		kfree(ptr);
	}
	

	/* TODO: 이제 본격적으로 연결리스트를 탐색하면서 하나씩 제거하도록 하시면 됩니다. */
	/* 다만, 제거를 하면서 연결리스트를 탐색하면 문제가 생길 것 같은데 어떤 방법으로 해결 가능한지 생각해보세요. */

	printk("Removing Module: BDLIST....\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("My name");
