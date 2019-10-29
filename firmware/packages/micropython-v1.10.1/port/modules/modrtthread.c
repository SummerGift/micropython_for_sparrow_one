/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Armink (armink.ztl@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/mpconfig.h"
#if MICROPY_PY_RTTHREAD

#include <rtthread.h>
#include <string.h>

#include "py/runtime.h"
#include "py/mpprint.h"

rt_bool_t rt_is_preempt_thread(void) {
    if (rt_interrupt_get_nest() || rt_critical_level()) {
        return RT_FALSE;
    } else {
        return RT_TRUE;
    }
}

STATIC void error_check(bool status, const char *msg) {
    if (!status) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, msg));
    }
}

STATIC mp_obj_t mod_is_preempt_thread(void) {
    return mp_obj_new_bool(rt_is_preempt_thread());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_is_preempt_thread_obj, mod_is_preempt_thread);

STATIC mp_obj_t mod_current_tid(void) {
    return MP_OBJ_NEW_SMALL_INT(rt_thread_self());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_current_tid_obj, mod_current_tid);

STATIC mp_obj_t mod_stacks_analyze(void) {
#ifdef RT_USING_FINSH
    extern long list_thread(void);
    list_thread();
#else
    mp_printf(&mp_plat_print, "Not available when FINSH module disable\n");
#endif

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_stacks_analyze_obj, mod_stacks_analyze);

STATIC mp_obj_t mod_list_device(void) {
    struct rt_device *device;
    struct rt_list_node *node;
    rt_ubase_t level;

    struct rt_object_information *info = rt_object_get_information(RT_Object_Class_Device);
    struct rt_list_node *list = &info->object_list;
    mp_obj_t mp_list = mp_obj_new_list(0, NULL);

    rt_enter_critical();

    for (node = list->next; node != list; node = node->next)
    {
        device = (struct rt_device *)(rt_list_entry(node, struct rt_object, list));

        mp_obj_tuple_t *t = mp_obj_new_tuple(2, NULL);
        t->items[0] = mp_obj_new_str(device->parent.name, strlen((char *)device->parent.name));
        t->items[1] = MP_OBJ_NEW_SMALL_INT((device->type <= RT_Device_Class_Unknown) ? device->type : RT_Device_Class_Unknown);
        mp_obj_list_append(mp_list, MP_OBJ_FROM_PTR(t));
    }

    rt_exit_critical();

    return mp_list;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_list_device_obj, mod_list_device);

STATIC mp_obj_t mod_http_ota(mp_obj_t http_url) {
    void mpy_http_ota(char *url);
    const char *url = mp_obj_str_get_str(http_url);
    mpy_http_ota(url);
    mp_printf(&mp_plat_print, "begin to http OTA");
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_http_ota_obj, mod_http_ota);

STATIC mp_obj_t wlan_connect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    #include "wlan_dev.h"
    #include "wlan_cmd.h"
    #include "drv_wlan.h"
    static int network_mode      = WIFI_STATION;

    enum { ARG_ssid, ARG_password, ARG_bssid };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_, MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_bssid, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_const_none} },
    };

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    const char *ssid = RT_NULL;
    const char *key = RT_NULL;
    size_t len;
    const char *p;

    // set parameters based on given args
    if (args[ARG_ssid].u_obj != mp_const_none) {
        p = mp_obj_str_get_data(args[ARG_ssid].u_obj, &len);
        ssid = p;
    }

    if (args[ARG_password].u_obj != mp_const_none) {
        p = mp_obj_str_get_data(args[ARG_password].u_obj, &len);
        key = p;
    }

    /* get wlan device */
    struct rt_wlan_device *wlan;
    wlan = (struct rt_wlan_device *)rt_device_find("w0");
    if (!wlan)
    {
        rt_kprintf("no wlan:%s device\n", "w0");
        return 0;
    }

    mp_printf(&mp_plat_print, "begin to connect WIFI\n");
    mp_printf(&mp_plat_print, "SSID: %s\n", ssid);
    mp_printf(&mp_plat_print, "key: %s\n", key);

    rt_wlan_init(wlan, WIFI_STATION);
    network_mode = WIFI_STATION;

    rt_wlan_info_init(&info, WIFI_STATION, SECURITY_WPA2_MIXED_PSK, ssid);
    error_check(rt_wlan_connect(wlan, &info, key) == RT_EOK, "Cannot connect to AP");
    rt_wlan_info_deinit(&info);
    mp_printf(&mp_plat_print, "Connect wifi successful.");

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_wlan_connect_obj, 3, wlan_connect);

STATIC const mp_rom_map_elem_t mp_module_rtthread_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_rtthread) },
    { MP_ROM_QSTR(MP_QSTR_is_preempt_thread), MP_ROM_PTR(&mod_is_preempt_thread_obj) },
    { MP_ROM_QSTR(MP_QSTR_current_tid), MP_ROM_PTR(&mod_current_tid_obj) },
    { MP_ROM_QSTR(MP_QSTR_stacks_analyze), MP_ROM_PTR(&mod_stacks_analyze_obj) },
    { MP_ROM_QSTR(MP_QSTR_list_device), MP_ROM_PTR(&mod_list_device_obj) },
    { MP_ROM_QSTR(MP_QSTR_http_ota), MP_ROM_PTR(&mod_http_ota_obj) },
    { MP_ROM_QSTR(MP_QSTR_wifi_join), MP_ROM_PTR(&mod_wlan_connect_obj) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_rtthread_globals, mp_module_rtthread_globals_table);

const mp_obj_module_t mp_module_rtthread = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_rtthread_globals,
};

#endif // MICROPY_PY_RTTHREAD
