/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: nil -*-*/

/***
  This file is part of systemd.

  Copyright 2013 Dave Reisner

  systemd is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  systemd is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with systemd; If not, see <http://www.gnu.org/licenses/>.
***/

#include "utf8.h"
#include "util.h"

static void test_utf8_is_printable(void) {
        assert_se(utf8_is_printable("ascii is valid\tunicode", 22));
        assert_se(utf8_is_printable("\342\204\242", 3));
        assert_se(!utf8_is_printable("\341\204", 2));
        assert_se(utf8_is_printable("ąę", 4));
}

static void test_ascii_is_valid(void) {
        assert_se(ascii_is_valid("alsdjf\t\vbarr\nba z"));
        assert_se(!ascii_is_valid("\342\204\242"));
        assert_se(!ascii_is_valid("\341\204"));
}

static void test_utf8_encoded_valid_unichar(void) {
        assert_se(utf8_encoded_valid_unichar("\342\204\242") == 3);
        assert_se(utf8_encoded_valid_unichar("\302\256") == 2);
        assert_se(utf8_encoded_valid_unichar("a") == 1);
        assert_se(utf8_encoded_valid_unichar("\341\204") < 0);
        assert_se(utf8_encoded_valid_unichar("\341\204\341\204") < 0);

}

static void test_utf16_to_utf8(void) {
        char *a = NULL;
        const uint16_t utf16[] = { 'a', 0xd800, 'b', 0xdc00, 'c', 0xd801, 0xdc37 };
        const char utf8[] = { 'a', 'b', 'c', 0xf0, 0x90, 0x90, 0xb7, 0 };

        a = utf16_to_utf8(utf16, 14);
        assert_se(a);
        assert_se(streq(a, utf8));

        free(a);
}

int main(int argc, char *argv[]) {
        test_utf8_is_printable();
        test_ascii_is_valid();
        test_utf8_encoded_valid_unichar();
        test_utf16_to_utf8();

        return 0;
}
