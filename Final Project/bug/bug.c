/**
 * @file    bug.c
 * @author  Chad Hirsch
 * @date    April 15 2019
 * @version 1.0
 * @brief  A loadable kernel module (LKM) that crashes due to a bug for the purpose of debugging with GDB. 
 * the /var/log/kern.log file when the module is loaded and removed.
 */

#include <linux/init.h>      // macros to mark up functions e.g. __init
#include <linux/module.h>    // core header for loading LKMs
#include <linux/kernel.h>    // contains kernel types, macros, functions

// The license type provides information (via modinfo) but it also affects kernel behavior.
// You can choose "Proprietary" for non-GPL code, but the kernel will be marked as "tainted".
// If you want anyone online to help you with an issue, your kernel better not be tainted.
MODULE_LICENSE("GPL");

// The author, description, and version of the module visible with modinfo
MODULE_AUTHOR("Chad Hirsch");
MODULE_DESCRIPTION("A simple Linux LKM with bug");
MODULE_VERSION("1.0");

// The variable name is declared as static to restrict it's scope to within the module
// You should avoid using global variables in kernel modules because they are shared kernel-wide.
static char *name = "bug"; // example LKM argument default is "world"

/**
 * The module_param(name, type, permissions) macro has three parameters:
 * 1) name, the parameter name displayed to the user and the variable name in the module
 * 2) type, the type of the parameter - one of byte, int, uint, long, ulong, short, ushort, bool,
 *    invool, or charap (char pointer)
 * 3) permissions, this is the access permissions to the parameter when using sysfs
 */

// param description charp = char pointer, defaults to "world"
module_param(name, charp, S_IRUGO); // S_IRUGO can be read/not changed
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

/** @brief The LKM initialization function
 * The static keyword restricts the visibility of the function to within this C file. The __init
 * macro means that for a built-in driver (not a LKM) the function is only used at initialization
 * time and that it can be discarded and its memory freed up after that point.
 * @return returns 0 if successful
 */
static int __init bug_init(void)
{
    // printk() is similar to printf() and you can call it from anywhere within LKM code.
    // The main difference is that you should specify a log level when you call the function.
    // The log levels are defined in linux/kern_levels.h.
    
    printk(KERN_INFO "This is right before I included a %s in the LKM to crash the program!\n", name);
    panic("There was a bug! Chad Hirsch is to blame! April 15 2019");
    return 0;
}

/** @brief The LKM cleanup function
 * Similar to the initialization function, it is static. The __exit macro notifies that if this code
 * is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit bug_exit(void)
{
    printk(KERN_INFO "Goodbye from the %s LKM!\n", name); //We never actually get here =(
}

/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 * identify the initialization function at insertion time and the cleanup function.
 *
 * The functions in the module can have whatever names you like; however, the same names must be
 * passed to the special macros module_init() and module_exit() at the very end of the module.
 */

// When this module is loaded, the hello_init() function executes
module_init(bug_init);

// When this module is unloaded, the hello_exit() function executes
module_exit(bug_exit);
