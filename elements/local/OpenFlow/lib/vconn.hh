/* Copyright (c) 2008, 2009 The Board of Trustees of The Leland Stanford
 * Junior University
 * 
 * We are making the OpenFlow specification and associated documentation
 * (Software) available for public use and benefit with the expectation
 * that others will use, modify and enhance the Software and contribute
 * those enhancements back to the community. However, since we would
 * like to make the Software available for broadest use, with as few
 * restrictions as possible permission is hereby granted, free of
 * charge, to any person obtaining a copy of this Software to deal in
 * the Software under the copyrights without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * The name and trademarks of copyright holder(s) may NOT be used in
 * advertising or publicity pertaining to the Software or any
 * derivatives without specific, written prior permission.
 */

#ifndef VCONN_H
#define VCONN_H 1

#include <click/config.h>

#include <stddef.h>
#include <stdint.h>
CLICK_DECLS

struct ofpbuf;
struct flow;
struct ofp_header;
struct ofp_stats_reply;
struct pvconn;
struct vconn;

void vconn_usage(bool active, bool passive, bool bootstrap);

/* Active vconns: virtual connections to OpenFlow devices. */
int vconn_open(const char *name, int min_version, struct vconn **);
void vconn_close(struct vconn *);
const char *vconn_get_name(const struct vconn *);
uint32_t vconn_get_ip(const struct vconn *);
bool vconn_is_reconnectable(const struct vconn *);
int vconn_connect(struct vconn *);
int vconn_recv(struct vconn *, struct ofpbuf **);
int vconn_send(struct vconn *, struct ofpbuf *);
int vconn_recv_xid(struct vconn *, uint32_t xid, struct ofpbuf **);
int vconn_transact(struct vconn *, struct ofpbuf *, struct ofpbuf **);

int vconn_open_block(const char *name, int min_version, struct vconn **);
int vconn_send_block(struct vconn *, struct ofpbuf *);
int vconn_recv_block(struct vconn *, struct ofpbuf **);

enum vconn_wait_type {
    WAIT_CONNECT,
    WAIT_RECV,
    WAIT_SEND
};
void vconn_wait(struct vconn *, enum vconn_wait_type);
void vconn_connect_wait(struct vconn *);
void vconn_recv_wait(struct vconn *);
void vconn_send_wait(struct vconn *);

/* Passive vconns: virtual listeners for incoming OpenFlow connections. */
int pvconn_open(const char *name, struct pvconn **);
void pvconn_close(struct pvconn *);
int pvconn_accept(struct pvconn *, int min_version, struct vconn **);
void pvconn_wait(struct pvconn *);

/* OpenFlow protocol utility functions. */
void *make_openflow(size_t openflow_len, uint8_t type, struct ofpbuf **);
void *make_openflow_xid(size_t openflow_len, uint8_t type,
                        uint32_t xid, struct ofpbuf **);
void *put_openflow(size_t openflow_len, uint8_t type, struct ofpbuf *);
void *put_openflow_xid(size_t openflow_len, uint8_t type, uint32_t xid,
                       struct ofpbuf *);
void update_openflow_length(struct ofpbuf *);
struct ofpbuf *make_flow_mod(uint16_t command, const struct flow *,
                             size_t actions_len);
struct ofpbuf *make_add_flow(const struct flow *, uint32_t buffer_id,
                             uint16_t max_idle, size_t actions_len);
struct ofpbuf *make_del_flow(const struct flow *);
struct ofpbuf *make_add_simple_flow(const struct flow *,
                                    uint32_t buffer_id, uint16_t out_port,
                                    uint16_t max_idle);
struct ofpbuf *make_buffered_packet_out(uint32_t buffer_id,
                                        uint16_t in_port, uint16_t out_port);
struct ofpbuf *make_unbuffered_packet_out(const struct ofpbuf *packet,
                                          uint16_t in_port, uint16_t out_port);
struct ofpbuf *make_echo_request(void);
struct ofpbuf *make_echo_reply(const struct ofp_header *rq);
int check_ofp_message(const struct ofp_header *, uint8_t type, size_t size);
int check_ofp_message_array(const struct ofp_header *, uint8_t type,
                            size_t size, size_t array_elt_size,
                            size_t *n_array_elts);

struct flow_stats_iterator {
    const uint8_t *pos, *end;
};
const struct ofp_flow_stats *flow_stats_first(struct flow_stats_iterator *,
                                              const struct ofp_stats_reply *);
const struct ofp_flow_stats *flow_stats_next(struct flow_stats_iterator *);

CLICK_ENDDECLS
#endif /* vconn.h */
