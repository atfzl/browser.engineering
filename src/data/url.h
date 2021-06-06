#pragma once

typedef struct url_t url_t;

url_t *url_init(const char *str);

void url_destroy(url_t *url);
