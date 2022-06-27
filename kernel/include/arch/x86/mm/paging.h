#ifndef X86_PAGING_H
#define X86_PAGING_H

struct page_directory_entry {
	unsigned p: 1,
		 rw: 1,
		 us: 1,
		 pwt: 1,
		 pcd: 1,
		 a: 1,
		 ignored: 1,
		 ps: 1,
		 ignored2: 4,
		 page_table_addr: 20;
} __attribute__((packed));

struct page_table_entry {
	unsigned p: 1,
		 rw: 1,
		 us: 1,
		 pwt: 1,
		 pcd: 1,
		 a: 1,
		 d: 1,
		 pat: 1,
		 g: 1,
		 ignored: 3,
		 page_frame_addr: 20;
} __attribute__((packed));

struct pf_errcode {
	unsigned p: 1, wr: 1, us: 1, rsvd: 1, id: 1, pk: 1, ss: 1, hlat: 1, reserved: 7, sgx: 1, reserved2: 15;
} __attribute__((packed));

#endif
