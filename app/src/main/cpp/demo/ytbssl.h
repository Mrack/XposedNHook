//
// Created by Mrack on 2024/4/28.
//

#ifndef XPOSEDNHOOK_YTBSSL_H
#define XPOSEDNHOOK_YTBSSL_H

void test_youtube();

void hook_SSL_CTX_set_custom_verify(void *ctx, int mode, void *callback);

#endif //XPOSEDNHOOK_YTBSSL_H
