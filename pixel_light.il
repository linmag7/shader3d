.inline pixel_light,16

	st	%o0,[%sp+2183]
    st	%o1,[%sp+2179]
    st  %o2,[%sp+2187]
	ld	[%sp+2183],%f0
    ld  [%sp+2187],%f6
    fmul8x16al %f0,%f6,%f2
    !fexpand %f0,%f2

	ld	[%sp+2179],%f1
    !fexpand %f1,%f4
    fmul8x16au %f1,%f6,%f4
    fpadd16 %f2,%f4,%f4
    fpack16 %f4,%f0


.end

.inline pixel_light_p,16


    ld [%o0],%f0
    ld [%o2],%f6
    fmul8x16al %f0,%f6,%f2
    !fexpand %f0,%f2
    ld [%o1],%f1
    fmul8x16au %f1,%f6,%f4
    !fexpand %f1,%f4
    fpadd16 %f2,%f4,%f4
    fpack16 %f4,%f0

.end



.inline pixel_filter,16


    ld [%o0+o1],%f0
    ld [%o0+02],%f2
    ld [%o0+03],%f4
    ld [%o0+04],%f6
    ld [%o0+05],%f8
    fexpand %f0,%f0
    fexpand %f2,%f2
    fexpand %f4,%f4
    fexpand %f6,%f6
    fexpand %f8,%f8

    fpadd16 %f2,%f4,%f4
    fpack16 %f4,%f0

.end

.inline pixel_env,16


    ld [%o0],%f0
    !ld [%o2],%f6
    ld [%o1],%f1
    !fmul8x16al %f0,%f6,%f2
    !fmul8x16au %f1,%f6,%f4
    fexpand %f0,%f2
    fexpand %f1,%f4
    fpadd16 %f2,%f4,%f4
    fpack16 %f4,%f0

.end

.inline pixel_phong,16


    ld [%o0],%f0
    ld [%o1],%f1
    !fmul8x16al %f0,%f6,%f2
    fmul8x16au %f0,%f1,%f4
    !fpadd16 %f2,%f4,%f4
    fpack16 %f4,%f0

.end


.inline set_gsr,0

    rd %gsr,%o0
    or %o0,24,%o0
    wr %o0,%gsr
    !nop
    !rd %gsr,%i3
.end

