#include <linux/interrupt.h>
#include <asm/env.h>
//#include <linux/irqchip.h>

static ssize_t
emem(u32 * data, size_t len)
{
	/* static unsigned const end = 2048; */
	static unsigned const start = 4;
	static unsigned ofs = start;
        ssize_t r = js_emem(ofs, data, len);
        ofs += r;
        return r;
}

void __init init_IRQ(void)
{
	//set_handle_irq(plat_irq_dispatch);
}

void __irq_entry do_IRQ(unsigned int irq)
{
	irq_enter();
	// check_stack_overflow();
	generic_handle_irq(irq);
	irq_exit();
}

static unsigned wait = 100;

asmlinkage void plat_irq_dispatch(void)
{
	unsigned int pending = evt_count(wait), i;
	for (i = 0; i < pending; i++) {
		unsigned int irq;
		emem(&irq, sizeof(irq));
		do_IRQ(irq);
	}
}
