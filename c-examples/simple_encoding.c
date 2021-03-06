// Encode the same tiny blank frame 30 times
//
// Usage:
// $ cargo build --release
// $ cc -Wall -I ../include -L../target/release -lrav1e simple_encoding.c -o simple_encoding

#include <rav1e.h>
#include <stdio.h>
#include <inttypes.h>

int main() {
    RaConfig *rac = rav1e_config_default(64*2, 48*2, 8,
        RA_CHROMA_SAMPLING_CS420,
        RA_CHROMA_SAMPLE_POSITION_UNKNOWN,
        (RaRational){ 1, 60 });

    rav1e_config_parse(rac, "speed", "9");

    RaContext *rax = rav1e_context_new(rac);
    RaFrame *f = rav1e_frame_new(rax);

    for (int i = 0; i < 30; i++) {
        printf("Sending frame\n");
        rav1e_send_frame(rax, f);
    }

    for (int i = 0; i < 30; i++) {
        RaPacket *p;
        printf("Encoding frame\n");
        rav1e_receive_packet(rax, &p);
        printf("Packet %"PRIu64"\n", p->number);
        rav1e_packet_unref(p);
    }

    rav1e_frame_unref(f);
    rav1e_context_unref(rax);
    rav1e_config_unref(rac);
}
