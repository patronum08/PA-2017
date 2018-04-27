#include "cpu/exec.h"

make_EHelper(add) {
    //TODO();
    if (decoding.opcode==0x02){
        id_dest->width=id_src->width=1;
        id_dest->val&=0xff;
        id_src->val&=0xff;
    }
    rtlreg_t lx_sign_dest0,lx_sign_src,lx_sign_dest1;

    rtl_msb(&lx_sign_dest0,&id_dest->val,id_dest->width);
    rtl_msb(&lx_sign_src,&id_src->val,id_src->width);

    id_dest->val+=id_src->val;

    rtl_msb(&lx_sign_dest1,&id_dest->val,id_dest->width);

    //rtl_sr(id_dest->reg,id_dest->width,&id_dest->val);
    operand_write(id_dest,&id_dest->val);

    cpu.EFLAGS.OF=(lx_sign_dest0==lx_sign_src)&&(lx_sign_dest0!=lx_sign_dest1);
    rtl_update_ZFSF(&id_dest->val,id_dest->width);
    cpu.EFLAGS.CF=((lx_sign_dest0||lx_sign_src)&&!lx_sign_dest1)||(lx_sign_dest0&&lx_sign_src)?0x1:0x0;

    print_asm_template2(add);
}

make_EHelper(sub) {
    //TODO();
    if ((decoding.opcode&0xff)==0x2d){
        id_dest->type=OP_TYPE_REG;
        id_dest->reg=0;
    }
    rtlreg_t lx_sign_dest0,lx_sign_src,lx_sign_dest1;
    cpu.EFLAGS.CF=decoding.dest.val>=decoding.src.val ? 0x0 : 0x1;

    rtl_msb(&lx_sign_dest0,&decoding.dest.val,decoding.dest.width);

    rtlreg_t ah=~decoding.src.val;
    rtl_msb(&lx_sign_src,&ah,decoding.src.width);
    decoding.dest.val-=decoding.src.val;
    //rtl_sr(id_dest->reg,id_dest->width,&id_dest->val);
    operand_write(id_dest,&id_dest->val);
    rtl_msb(&lx_sign_dest1,&decoding.dest.val,decoding.dest.width);

    cpu.EFLAGS.OF=(lx_sign_dest0==lx_sign_src)&&(lx_sign_dest0!=lx_sign_dest1);

    rtl_update_ZFSF(&decoding.dest.val,decoding.dest.width);      // set ZF and SF
    //if (decoding.dest.val==0x80000000&&decoding.src.val==0x80000000){    // set CF
    //    cpu.EFLAGS.CF=0x0;
    //}
    //else 
    //strcpy(id_dest->str,id_src->str);
    print_asm_template2(sub);
}

make_EHelper(cmp) {
    //TODO();
    if (decoding.is_operand_size_16){
        id_src->width=id_dest->width=2;
    }
    if ((decoding.opcode&0xff)==0x83){
        rtl_sext(&id_src->val,&id_src->val,1);
        if(decoding.is_operand_size_16){
            id_dest->val&=0x0000ffff;
            id_src->val&=0x0000ffff;
        }
    }
    else if ((decoding.opcode&0xff)==0x3c){
        id_src->width=id_dest->width=1;
        id_dest->type=OP_TYPE_REG;
        id_dest->reg=0;
        id_dest->val=cpu.eax&0x000000ff;
    }
    else if ((decoding.opcode&0xff)==0x3d){
        //id_src->width=id_dest->width=2;
        id_dest->type=OP_TYPE_REG;
        id_dest->reg=0;
        id_dest->val=cpu.eax;
        if (decoding.is_operand_size_16){
            id_dest->width=id_src->width=2;
            id_dest->val&=0xffff;
            id_src->val&=0xffff;
        }
        //id_dest->val&=0x0000ffff;
    }
    else if ((decoding.opcode&0xff)==0x38||(decoding.opcode&0xff)==0x80||(decoding.opcode&0xff)==0x3a){
        id_src->width=id_dest->width=1;
        id_src->val&=0xff;
        id_dest->val&=0xff ;
    }
    //if ((decoding.opcode&0xff)==0x83){
    //    cpu.EFLAGS.CF=(int)id_dest->val>=(int)id_src->val?0x0:0x1;
   // }
    cpu.EFLAGS.CF=decoding.dest.val>=decoding.src.val ? 0x0 : 0x1;
    rtlreg_t lx_sign_dest0,lx_sign_src,lx_sign_dest1;

    rtl_msb(&lx_sign_dest0,&decoding.dest.val,decoding.dest.width);

    rtlreg_t ah=~decoding.src.val;
    rtl_msb(&lx_sign_src,&ah,decoding.src.width);
    decoding.dest.val-=decoding.src.val;
    rtl_msb(&lx_sign_dest1,&decoding.dest.val,decoding.dest.width);

    cpu.EFLAGS.OF=(lx_sign_dest0==lx_sign_src)&&(lx_sign_dest0!=lx_sign_dest1);

    rtl_update_ZFSF(&decoding.dest.val,decoding.dest.width);      // set ZF and SF
    //if (decoding.dest.val==0x80000000&&decoding.src.val==0x80000000){    // set CF
    //    cpu.EFLAGS.CF=0x0;
    //}
    //else 
    cpu.EFLAGS.SF=lx_sign_dest1;
    print_asm_template2(cmp);
}

make_EHelper(inc) {
    //TODO();
    rtlreg_t lx_sign_dest0,lx_sign_src,lx_sign_dest1;

    id_src->val=0x1;
    rtl_msb(&lx_sign_dest0,&id_dest->val,id_dest->width);
    rtl_msb(&lx_sign_src,&id_src->val,id_src->width);

    id_dest->val+=id_src->val;

    rtl_msb(&lx_sign_dest1,&id_dest->val,id_dest->width);

    //rtl_sr(id_dest->reg,id_dest->width,&id_dest->val);
    operand_write(id_dest,&id_dest->val);
    cpu.EFLAGS.OF=(lx_sign_dest0==lx_sign_src)&&(lx_sign_dest0!=lx_sign_dest1);
    rtl_update_ZFSF(&id_dest->val,id_dest->width);
    //cpu.EFLAGS.CF=((lx_sign_dest0||lx_sign_src)&&!lx_sign_dest1)||(lx_sign_dest0&&lx_sign_src)?0x1:0x0;

    print_asm_template1(inc);
}

make_EHelper(dec) {    // FF /1 DEC r/m16 r/m32
    //TODO();
    rtlreg_t lx_sign_dest0,lx_sign_src,lx_sign_dest1;
    id_src->val=1;
    rtl_msb(&lx_sign_dest0,&decoding.dest.val,decoding.dest.width);

    rtlreg_t ah=~decoding.src.val;
    rtl_msb(&lx_sign_src,&ah,decoding.src.width);
    decoding.dest.val-=decoding.src.val;
    //rtl_sr(id_dest->reg,id_dest->width,&id_dest->val);
    operand_write(id_dest,&id_dest->val);
    rtl_msb(&lx_sign_dest1,&decoding.dest.val,decoding.dest.width);

    cpu.EFLAGS.OF=(lx_sign_dest0==lx_sign_src)&&(lx_sign_dest0!=lx_sign_dest1);

    rtl_update_ZFSF(&decoding.dest.val,decoding.dest.width);      // set ZF and SF
    //if (decoding.dest.val==0x80000000&&decoding.src.val==0x80000000){    // set CF
    //    cpu.EFLAGS.CF=0x0;
    //}
    //else 
    //cpu.EFLAGS.CF=decoding.dest.val>=decoding.src.val ? 0x0 : 0x1;
    //strcpy(id_dest->str,id_src->str);
    print_asm_template1(dec);
}

make_EHelper(neg) {
    //TODO();
    if (id_dest->val==0){
        cpu.EFLAGS.CF=0;
    }
    else cpu.EFLAGS.CF=1;
    id_dest->val=-id_dest->val;
    rtl_sr(id_dest->reg,id_dest->width,&id_dest->val);
    print_asm_template1(neg);
}

make_EHelper(adc) {
    rtl_add(&t2, &id_dest->val, &id_src->val);
    //rtl_get_CF(&t1);
    t1=cpu.EFLAGS.CF;
    rtl_add(&t2, &t2, &t1);
    operand_write(id_dest, &t2);

    rtl_update_ZFSF(&t2, id_dest->width);

    rtl_sltu(&t0, &t2, &id_dest->val);
    // rtl_set_CF(&t0);
    cpu.EFLAGS.CF=t0;
    rtl_xor(&t0, &id_dest->val, &id_src->val);
    rtl_not(&t0);
    rtl_xor(&t1, &id_dest->val, &t2);
    rtl_and(&t0, &t0, &t1);
    rtl_msb(&t0, &t0, id_dest->width);
    //rtl_set_OF(&t0);
    cpu.EFLAGS.OF=t0;
    print_asm_template2(adc);
}

make_EHelper(sbb) {
    if (decoding.is_operand_size_16){
        id_dest->width=id_src->width=2;
        id_dest->val&=0x0000ffff;
        id_src->val&=0x0000ffff;
    }
    if ((decoding.opcode&0xff)==0x83){
        rtl_sext(&id_src->val,&id_src->val,1);
        id_src->width=1;
    }
    if ((decoding.opcode&0xff)==0x80){
        id_dest->width=id_src->width=1;
    }
    rtl_sub(&t2, &id_dest->val, &id_src->val);
    //rtl_get_CF(&t1);
    t1=cpu.EFLAGS.CF;
    rtl_sub(&t2, &t2, &t1);
    operand_write(id_dest, &t2);

    rtl_update_ZFSF(&t2, id_dest->width);

    rtl_sltu(&t0, &id_dest->val, &t2);
    //rtl_set_CF(&t0);    //!
    cpu.EFLAGS.CF=t0;
    rtl_xor(&t0, &id_dest->val, &id_src->val);
    rtl_xor(&t1, &id_dest->val, &t2);
    rtl_and(&t0, &t0, &t1);
    rtl_msb(&t0, &t0, id_dest->width);
    //rtl_set_OF(&t0);
    cpu.EFLAGS.OF=t0;
    print_asm_template2(sbb);
}

make_EHelper(mul) {
    rtl_lr(&t0, R_EAX, id_dest->width);
    rtl_mul(&t0, &t1, &id_dest->val, &t0);

    switch (id_dest->width) {
        case 1:
            rtl_sr_w(R_AX, &t1);
            break;
        case 2:
            rtl_sr_w(R_AX, &t1);
            rtl_shri(&t1, &t1, 16);
            rtl_sr_w(R_DX, &t1);
            break;
        case 4:
            rtl_sr_l(R_EDX, &t0);
            rtl_sr_l(R_EAX, &t1);
            break;
        default: assert(0);
    }

    print_asm_template1(mul);
}

// imul with one operand
make_EHelper(imul1) {
    rtl_lr(&t0, R_EAX, id_dest->width);
    rtl_imul(&t0, &t1, &id_dest->val, &t0);

    switch (id_dest->width) {
        case 1:
            rtl_sr_w(R_AX, &t1);
            break;
        case 2:
            rtl_sr_w(R_AX, &t1);
            rtl_shri(&t1, &t1, 16);
            rtl_sr_w(R_DX, &t1);
            break;
        case 4:
            rtl_sr_l(R_EDX, &t0);
            rtl_sr_l(R_EAX, &t1);
            break;
        default: assert(0);
    }

    print_asm_template1(imul);
}

// imul with two operands
make_EHelper(imul2) {
    rtl_sext(&id_src->val, &id_src->val, id_src->width);
    rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

    rtl_imul(&t0, &t1, &id_dest->val, &id_src->val);
    operand_write(id_dest, &t1);

    print_asm_template2(imul);
}

// imul with three operands
make_EHelper(imul3) {
    rtl_sext(&id_src->val, &id_src->val, id_src->width);
    rtl_sext(&id_src2->val, &id_src2->val, id_src->width);
    rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

    rtl_imul(&t0, &t1, &id_src2->val, &id_src->val);
    operand_write(id_dest, &t1);

    print_asm_template3(imul);
}

make_EHelper(div) {
    switch (id_dest->width) {
        case 1:
            rtl_li(&t1, 0);
            rtl_lr_w(&t0, R_AX);
            break;
        case 2:
            rtl_lr_w(&t0, R_AX);
            rtl_lr_w(&t1, R_DX);
            rtl_shli(&t1, &t1, 16);
            rtl_or(&t0, &t0, &t1);
            rtl_li(&t1, 0);
            break;
        case 4:
            rtl_lr_l(&t0, R_EAX);
            rtl_lr_l(&t1, R_EDX);
            break;
        default: assert(0);
    }

    rtl_div(&t2, &t3, &t1, &t0, &id_dest->val);

    rtl_sr(R_EAX, id_dest->width, &t2);
    if (id_dest->width == 1) {
        rtl_sr_b(R_AH, &t3);
    }
    else {
        rtl_sr(R_EDX, id_dest->width, &t3);
    }

    print_asm_template1(div);
}

make_EHelper(idiv) {
    rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

    switch (id_dest->width) {
        case 1:
            rtl_lr_w(&t0, R_AX);
            rtl_sext(&t0, &t0, 2);
            rtl_msb(&t1, &t0, 4);
            rtl_sub(&t1, &tzero, &t1);
            break;
        case 2:
            rtl_lr_w(&t0, R_AX);
            rtl_lr_w(&t1, R_DX);
            rtl_shli(&t1, &t1, 16);
            rtl_or(&t0, &t0, &t1);
            rtl_msb(&t1, &t0, 4);
            rtl_sub(&t1, &tzero, &t1);
            break;
        case 4:
            rtl_lr_l(&t0, R_EAX);
            rtl_lr_l(&t1, R_EDX);
            break;
        default: assert(0);
    }

    rtl_idiv(&t2, &t3, &t1, &t0, &id_dest->val);

    rtl_sr(R_EAX, id_dest->width, &t2);
    if (id_dest->width == 1) {
        rtl_sr_b(R_AH, &t3);
    }
    else {
        rtl_sr(R_EDX, id_dest->width, &t3);
    }

    print_asm_template1(idiv);
}