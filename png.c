#include <uefi.h>
#include "embdedded_file.h"

/**
 * Display GIF image
 */
int main(int argc, char **argv)
{
    efi_status_t status;
    efi_guid_t gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    efi_gop_t *gop = NULL;
    int i;

    /* set video mode */
    status = BS->LocateProtocol(&gopGuid, NULL, (void**)&gop);
    if(!EFI_ERROR(status) && gop) {
        status = gop->SetMode(gop, 0);
        ST->ConOut->Reset(ST->ConOut, 0);
        ST->StdErr->Reset(ST->StdErr, 0);
        if(EFI_ERROR(status)) {
            fprintf(stderr, "unable to set video mode\n");
            return 0;
        }
    } else {
        fprintf(stderr, "unable to get graphics output protocol\n");
        return 0;
    }

    // For each frame...
    for (i = 0; i < GIF_FRAMES; i++) {
        /* display image */
        gop->Blt(gop, gif_frames[i], EfiBltBufferToVideo, 0, 0, (gop->Mode->Information->HorizontalResolution - GIF_WIDTH) / 2,
            (gop->Mode->Information->VerticalResolution - GIF_HEIGHT) / 2, GIF_WIDTH, GIF_HEIGHT, 0);
        usleep(1e6 / GIF_FPS);
    }

    return 0;
}
