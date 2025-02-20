#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <Cocoa/Cocoa.h>

#include "../core/platform.h"

static NSAutoreleasePool *g_autoreleasepool = NULL;

/**
 * @brief 创建macOS应用的主菜单栏
 * @note 该函数会构建包含标准应用菜单（包含Quit项）的菜单栏结构
 *       菜单项绑定终止应用的方法@selector(terminate:)，并设置快捷键为"q"
 */
static void create_menubar(void) {
    NSMenu *menu_bar, *app_menu;
    NSMenuItem *app_menu_item, *quit_menu_item;
    NSString *app_name, *quit_title;

    /* 创建主菜单栏容器 */
    menu_bar = [[[NSMenu alloc] init] autorelease];
    [NSApp setMainMenu:menu_bar];

    /* 创建应用主菜单项并添加到菜单栏 */
    app_menu_item = [[[NSMenuItem alloc] init] autorelease];
    [menu_bar addItem:app_menu_item];

    /* 创建应用子菜单并关联到主菜单项 */
    app_menu = [[[NSMenu alloc] init] autorelease];
    [app_menu_item setSubmenu:app_menu];

    /* 构建带应用名称的Quit菜单项 */
    app_name = [[NSProcessInfo processInfo] processName];
    quit_title = [@"Quit " stringByAppendingString:app_name];
    quit_menu_item = [[[NSMenuItem alloc] initWithTitle:quit_title
                                                 action:@selector(terminate:)
                                          keyEquivalent:@"q"] autorelease];
    [app_menu addItem:quit_menu_item];
}

/**
 * @brief 初始化Cocoa应用实例
 * @note 如果NSApp未初始化，则执行以下操作：
 *       1. 创建全局自动释放池
 *       2. 获取共享应用实例
 *       3. 设置应用为常规窗口化应用策略
 *       4. 创建菜单栏
 *       5. 完成应用启动流程
 */
static void create_application(void) {
    if (NSApp == nil) {
        g_autoreleasepool = [[NSAutoreleasePool alloc] init];
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        create_menubar();
        [NSApp finishLaunching];
    }
}

/**
 * @brief 平台初始化入口函数
 * @note 执行Cocoa应用框架的基础初始化工作
 *       主要职责是调用create_application创建应用实例
 */
void platform_initialize(void) {
    printf("Initializing platform...");
    create_application();
}