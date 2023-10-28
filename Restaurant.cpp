#include "main.h"

extern int MAXSIZE;

class imp_res : public Restaurant
{  
	public:
		int CustomerInTable;
		int ChuThuatSu;
		int OanLinh;
		int CustomerInQueue;
		int CountShellSort;
		customer* CurrentCustomerInTable;
		customer* QueueHead;
		customer* QueueTail;
		customer* OrderHead;
		customer* OrderTail;
		
	public:	

		imp_res() {
			CustomerInTable = 0;
			ChuThuatSu = 0;
			OanLinh = 0;
			CustomerInQueue = 0;
			CountShellSort = 0;
			CurrentCustomerInTable = nullptr;
			QueueHead = nullptr;
			QueueTail = nullptr;
			OrderHead = nullptr;
			OrderTail = nullptr;
		};
		
		~imp_res(){
			ChuThuatSu = 0;
			OanLinh = 0;
			CustomerInQueue = 0;
			CountShellSort = 0;
			while (OrderHead != nullptr){
				customer* tmp = OrderHead;
				OrderHead = OrderHead->next;
				if (OrderHead != nullptr){ // Con khach hang thi set lai tro prev de xoa khong loi
					OrderHead->prev = nullptr;
				}
				else OrderTail = nullptr; // OrderInTableHead = null thi tail cung se la null 
				tmp->next = nullptr;
				delete tmp;
			}
			while (QueueHead != nullptr){
				customer* tmp = QueueHead;
				QueueHead = QueueHead->next;
				if (QueueHead != nullptr){
					QueueHead->prev = nullptr;
				}
				else QueueTail = nullptr;
				tmp->next = nullptr;
				delete tmp;
			}
			for (int i = 1; i <= CustomerInTable; i++){
				customer* DeleteCustomer = CurrentCustomerInTable;
				CurrentCustomerInTable->prev->next = CurrentCustomerInTable->next;
				CurrentCustomerInTable->next->prev = CurrentCustomerInTable->prev;
				CurrentCustomerInTable = CurrentCustomerInTable->next;
				DeleteCustomer->prev = nullptr;
				DeleteCustomer->next = nullptr;
				delete DeleteCustomer;
			}
			CurrentCustomerInTable = nullptr;
			CustomerInTable = 0;
		} 
		
		void RED(string name, int energy){
			if (energy == 0) { // Khong tiep khach hang co energy = 0
				return;
			}
			if (CustomerInTable >= MAXSIZE && CustomerInQueue >= MAXSIZE){ // Khach trong ban va hang doi da full
				return;
			}
			customer* check = OrderHead; // Kiem tra ten da co trong ban chua
			while (check != nullptr){
				if (check->name == name){
					return; // Co thi khong them khach hang nay
				}
				check = check->next;
			}
			if (CustomerInTable < MAXSIZE){ // Them khach trong ban chua full
				customer* cus = new customer(name, energy, nullptr, nullptr);
				if (energy > 0){
					ChuThuatSu += energy;
				}
				else OanLinh += energy;
				customer* temp = new customer(); // deep copy cho List thu tu khach trong ban
				temp->name = cus->name;
				temp->energy = cus->energy;
				temp->prev = nullptr;
				temp->next = nullptr;
				if (CustomerInTable == 0){ // Trong ban chua co ai
					cus->prev = cus;
					cus->next = cus;
					CurrentCustomerInTable = cus;
					OrderHead = temp;
					OrderTail = temp;
				}
				else {
					OrderTail->next = temp; // Them vao List thu tu cua khach trong ban
					temp->prev = OrderTail;
					OrderTail = temp;
					if (CustomerInTable > 0 && CustomerInTable < MAXSIZE / 2){ // So luong khach trong ban nho hon MAXSIZE / 2
						if (energy >= CurrentCustomerInTable->energy){ // Energy lon hon Energy cua X them vao phia Thuan KDH
							cus->prev = CurrentCustomerInTable;
							cus->next = CurrentCustomerInTable->next;
							CurrentCustomerInTable->next->prev = cus;
							CurrentCustomerInTable->next = cus;
							CurrentCustomerInTable = cus;
						}
						else {
							cus->prev = CurrentCustomerInTable->prev; // Them vao phia nguoc chieu KDH
							cus->next = CurrentCustomerInTable;
							CurrentCustomerInTable->prev->next = cus;
							CurrentCustomerInTable->prev = cus;
							CurrentCustomerInTable = cus;
						}
					}
					else if (CustomerInTable >= MAXSIZE / 2 && CustomerInTable < MAXSIZE){// So luong khach vuot qua MAXSIZE / 2
						customer* tmp = CurrentCustomerInTable;
						int res = energy - tmp->energy; // RES
						for (int i = 1; i <= CustomerInTable; i++){
							if (abs(res) < abs(energy - tmp->energy)){ // Tim tri tuyet doi lon nhat cua energy va energy cua 
								res = energy - tmp->energy; // khach trong ban -> co nhiieu lay thang dau tien
								CurrentCustomerInTable = tmp;
							}
							tmp = tmp->next;
						}
						if (res < 0){ // them vao nguoc chieu KDH
							cus->prev = CurrentCustomerInTable->prev;
							cus->next = CurrentCustomerInTable;
							CurrentCustomerInTable->prev->next = cus;
							CurrentCustomerInTable->prev = cus;
							CurrentCustomerInTable = cus;
						}
						else { // them vao cung chieu KDH
							cus->prev = CurrentCustomerInTable;
							cus->next = CurrentCustomerInTable->next;
							CurrentCustomerInTable->next->prev = cus;
							CurrentCustomerInTable->next = cus;
							CurrentCustomerInTable = cus;
						}
					}
				}
				CustomerInTable += 1; // Tang so khach trong ban neu duoc them
			}
			else if (CustomerInTable == MAXSIZE && CustomerInQueue < MAXSIZE){ // Trong ban da full thi them hang doi
				customer* cus = new customer(name, energy, nullptr, nullptr);
				if (energy > 0){
					ChuThuatSu += energy;
				}
				else OanLinh += energy;
				customer* temp = new customer(); // deep copy de them vao thu tu trong hang doi
				temp->name = cus->name;
				temp->energy = cus->energy;
				temp->prev = nullptr;
				temp->next = nullptr;
				OrderTail->next = temp;
				temp->prev = OrderTail;
				OrderTail = temp;
				if (CustomerInQueue == 0){
					QueueHead = cus;
					QueueTail = cus;
				}
				else {
					QueueTail->next = cus;
					cus->prev = QueueTail;
					QueueTail = cus;
				}
				CustomerInQueue += 1; // Tang so luong khach trong hang doi len 1
			}
		}

		void DICHUYEN(){
			while (CustomerInTable < MAXSIZE){
				if (QueueHead == nullptr) {
					return;
				}
				customer* tmp = QueueHead; // Them theo thu tu FIFO
				string name = QueueHead->name;
				int energy = QueueHead->energy;

				QueueHead = QueueHead->next;
				if (QueueHead != nullptr){
					QueueHead->prev = nullptr;
				}
				else QueueTail = nullptr;
				tmp->next = nullptr;
				delete tmp;
				CustomerInQueue -= 1; // Xoa so luong khach trong hang doi di 1
				/////////////////////////////////////////////////////////
				customer* cus = new customer(name, energy, nullptr, nullptr);

				if (CustomerInTable == 0){ // Trong ban chua co ai
					cus->prev = cus;
					cus->next = cus;
					CurrentCustomerInTable = cus;
				}
				else {
					if (CustomerInTable > 0 && CustomerInTable < MAXSIZE / 2){ // So luong khach trong ban nho hon MAXSIZE / 2
						if (energy >= CurrentCustomerInTable->energy){ // Energy lon hon Energy cua X them vao phia Thuan KDH
							cus->prev = CurrentCustomerInTable;
							cus->next = CurrentCustomerInTable->next;
							CurrentCustomerInTable->next->prev = cus;
							CurrentCustomerInTable->next = cus;
							CurrentCustomerInTable = cus;
						}
						else {
							cus->prev = CurrentCustomerInTable->prev; // Them vao phia nguoc chieu KDH
							cus->next = CurrentCustomerInTable;
							CurrentCustomerInTable->prev->next = cus;
							CurrentCustomerInTable->prev = cus;
							CurrentCustomerInTable = cus;
						}
					}
					else if (CustomerInTable >= MAXSIZE / 2 && CustomerInTable < MAXSIZE){// So luong khach vuot qua MAXSIZE / 2
						tmp = CurrentCustomerInTable;
						int res = energy - tmp->energy; // RES
						for (int i = 1; i <= CustomerInTable; i++){
							if (abs(res) < abs(energy - tmp->energy)){ // Tim tri tuyet doi lon nhat cua energy va energy cua 
								res = energy - tmp->energy; // khach trong ban -> co nhiieu lay thang dau tien
								CurrentCustomerInTable = tmp;
							}
							tmp = tmp->next;
						}
						if (res < 0){ // them vao nguoc chieu KDH
							cus->prev = CurrentCustomerInTable->prev;
							cus->next = CurrentCustomerInTable;
							CurrentCustomerInTable->prev->next = cus;
							CurrentCustomerInTable->prev = cus;
							CurrentCustomerInTable = cus;
						}
						else { // them vao cung chieu KDH
							cus->prev = CurrentCustomerInTable;
							cus->next = CurrentCustomerInTable->next;
							CurrentCustomerInTable->next->prev = cus;
							CurrentCustomerInTable->next = cus;
							CurrentCustomerInTable = cus;
						}
					}
				}
				CustomerInTable += 1; // Tang so khach trong ban neu duoc them
			}
		}

		customer* KIEMTRAKHACHTRONGBAN(string name){
			customer* tmp = CurrentCustomerInTable;
			for (int i = 1; i <= CustomerInTable; i++){
				if (tmp->name == name){
					return tmp;
				}
				tmp = tmp->next;
			}
			return nullptr;
		}

		customer* KIEMTRAKHACHTRONGHANGDOI(string name){
			customer* tmp = QueueHead;
			while (tmp != nullptr){
				if (tmp->name == name){
					return tmp;
				}
				tmp = tmp->next;
			}
			return nullptr;
		}

		void BLUE(int num)
		{	
			if (num >= CustomerInTable || num > MAXSIZE){ // Xoa het tat ca khach trong ban
				customer* run = OrderHead;
				while (run != nullptr){
					customer* khach_trong_ban = KIEMTRAKHACHTRONGBAN(run->name);
					if (khach_trong_ban != nullptr){
						if (run->energy > 0){
							ChuThuatSu -= run->energy;
						}
						else OanLinh -= run->energy;
						customer* tmp = run;
						if (run->name == OrderHead->name){
							OrderHead = OrderHead->next;
							if (OrderHead != nullptr){
								OrderHead->prev = nullptr;
							}
							else OrderTail = nullptr;
						}
						else if (run->name == OrderTail->name){
							OrderTail = OrderTail->prev;
							if (OrderTail != nullptr){
								OrderTail->next = nullptr;
							}
							else OrderHead = nullptr;
						}
						else {
							tmp->prev->next = tmp->next;
							tmp->next->prev = tmp->prev;
						}
						run = run->next;
						tmp->prev = nullptr;
						tmp->next = nullptr;
						delete tmp;
					}
					else run = run->next;
				}
				for (int i = 1; i <= CustomerInTable; i++){ // Xoa khach trong ban
					customer* DeleteCustomer = CurrentCustomerInTable;
					CurrentCustomerInTable->prev->next = CurrentCustomerInTable->next;
					CurrentCustomerInTable->next->prev = CurrentCustomerInTable->prev;
					CurrentCustomerInTable = CurrentCustomerInTable->next;
					DeleteCustomer->prev = nullptr;
					DeleteCustomer->next = nullptr;
					delete DeleteCustomer;
				}
				CurrentCustomerInTable = nullptr;
				CustomerInTable = 0; // Set lai khach trong ban la 0
			}
			else{
				customer* run = OrderHead;
				for (int i = 1; i <= num; i++){
					while (KIEMTRAKHACHTRONGBAN(run->name) == nullptr){
						run = run->next;
					}
					customer* khach_trong_ban = KIEMTRAKHACHTRONGBAN(run->name);
					if (khach_trong_ban->energy > 0){
						ChuThuatSu -= khach_trong_ban->energy;
					}
					else OanLinh -= khach_trong_ban->energy;
					////////////////////////////////
					khach_trong_ban->prev->next = khach_trong_ban->next;
					khach_trong_ban->next->prev = khach_trong_ban->prev;
					if (khach_trong_ban->energy < 0){
						CurrentCustomerInTable = khach_trong_ban->prev;
					}
					else CurrentCustomerInTable = khach_trong_ban->next;
					khach_trong_ban->prev = nullptr;
					khach_trong_ban->next = nullptr;
					delete khach_trong_ban;
					/////////////////////////
					customer* tmp = run;
					if (run->name == OrderHead->name){
						OrderHead = OrderHead->next;
						if (OrderHead != nullptr){
							OrderHead->prev = nullptr;
						}
						else OrderTail = nullptr;
					}
					else if (run->name == OrderTail->name){
						OrderTail = OrderTail->prev;
						if (OrderTail != nullptr){
							OrderTail->next = nullptr;
						}
						else OrderHead = nullptr;
					}
					else {
						tmp->prev->next = tmp->next;
						tmp->next->prev = tmp->prev;
					}
					run = run->next;
					tmp->prev = nullptr;
					tmp->next = nullptr;
					delete tmp;
				}
				CustomerInTable -= num;
			}	
			DICHUYEN();
		}

		void swap(customer* a, customer* b){ // Doi value cua 2 cus
			int energy = a->energy;
			string name = a->name;
			a->energy = b->energy;
			a->name = b->name;
			b->energy = energy;
			b->name = name;
		}

		void inssort2(customer* head, int n, int incr){
			for (int i = incr; i < n; i += incr){ // Duyet theo phan vung incr
				customer* front = head;
				customer* back = head;
				for (int k = 0; k < i; k++){
					if (front == nullptr){
						break;
					}
					front = front->next;
				}
				for (int k = 0; k < i - incr; k++){
					if (back == nullptr){
						break;
					}
					back = back->next; // back chay sau front incr cus
				}
				for (int j = i; j >= incr; j -= incr){
					if (front != nullptr && back != nullptr && abs(front->energy) > abs(back->energy)){ // Theo chieu giam
						CountShellSort++; ////Dem so lan chuyen doi vi tri///////////////////// dan cua tri tuyet doi energy
						swap(front, back);
					}
					if (front != nullptr && back != nullptr && abs(front->energy) == abs(back->energy)){ // Theo chieu giam
						customer* tmp = OrderHead;
						int check = 0;
						while (tmp != nullptr){
							if (tmp->name == front->name){
								check = 1;
								break;
							}
							if (tmp->name == back->name){
								check = 2;
								break;
							}
							tmp = tmp->next;
						}
						if (check == 1){
							CountShellSort++; ////Dem so lan chuyen doi vi tri///////////////////// dan cua tri tuyet doi energy
							swap(front, back);
						}
					}
					for (int k = 0; k < incr; k++){
						if (front == nullptr || back == nullptr){ // Tang frong va back len incr cus giu dung khoang cach
							break; ////////////////////////////////// trong giai thuat shellsort
						}
						front = front->prev;
						back = back->prev;
					}
				}
			}
		}

		void PURPLE()
		{
			if (QueueHead == nullptr){ // Hang cho khong co khach -> khong lam gi
				return;
			}
			else {
				CountShellSort = 0;
				customer* tmp = QueueHead;
				customer* end = QueueHead;
				while (tmp != nullptr){ // Tim vi khach co tri tuyet doi energy cao nhat trong hang cho
					if (abs(end->energy) < abs(tmp->energy)){
						end = tmp;
					}
					if (abs(end->energy) == abs(tmp->energy)){
						customer* chay = OrderHead;
						int check = 0;
						while (chay != nullptr){
							if (chay->name == end->name){
								check = 1;
								break;
							}
							if (chay->name == tmp->name){
								check = 2;
								break;
							}
							chay = chay->next;
						}
						if (check == 1){
							end = tmp;
						}
					}
					tmp = tmp->next;
				}
				customer* Match = end->next; // Phan duoi sap xep xong se noi vao
				end->next = nullptr; // Chi sap xep tu dau hang den vi khach co |energy| lon nhat
				int size = 0; // So khach hang tu dau hang doi den end
				tmp = QueueHead;
				while (tmp != nullptr){
					size += 1;
					tmp = tmp->next;
				}
				for (int i = size / 2; i > 2; i /= 2){ // shell sort 
					for (int j = 0; j < i; j++){
						customer* sublist = QueueHead; // Khoi tao ban dau
						for (int k = 0; k < j; k++){
							sublist = sublist->next; // Vi tri bat dau sap xep theo j -> tien ra j cus
						}
						inssort2(sublist, size - j, i); 
					}
				}
    			inssort2(QueueHead, size, 1);
				end->next = Match; // Noi danh sach lien ket
				CountShellSort = CountShellSort % MAXSIZE; // Chia su so lan chuyen doi vi tri
				if (CountShellSort != 0){
					BLUE(CountShellSort);
				}
			}
		}
		
		void REVERSAL()
		{
			if (CurrentCustomerInTable == nullptr) return;
			customer* tmp = CurrentCustomerInTable;
			string name = CurrentCustomerInTable->name;
			customer* line1 = nullptr;
			customer* line2 = nullptr;
			for (int i = 1; i <= CustomerInTable; i++){
				customer* temp = new customer();
				temp->name = tmp->name;
				temp->energy = tmp->energy;
				temp->prev = nullptr;
				temp->next = nullptr;
				if (tmp->energy * CurrentCustomerInTable->energy > 0){
					if (line1 == nullptr){
						line1 = temp;
					}
					else {
						line1->next = temp;
						temp->prev = line1;
						line1 = temp;
					}
				}
				else {
					if (line2 == nullptr){
						line2 = temp;
					}
					else {
						line2->next = temp;
						temp->prev = line2;
						line2 = temp;
					}
				}
				tmp = tmp->prev;
			}
			customer* line_1_backup = line1;
			customer* line_2_backup = line2;
			for (int i = 1; i <= CustomerInTable; i++){
				if (line1 != nullptr && tmp->energy * line1->energy > 0){
					tmp->energy = line1->energy;
					tmp->name = line1->name;
					line1 = line1->prev;
				}
				else if (line2 != nullptr){
					tmp->energy = line2->energy;
					tmp->name = line2->name;
					line2 = line2->prev;
				}
				tmp = tmp->prev;
			}
			while (CurrentCustomerInTable->name != name){
				CurrentCustomerInTable = CurrentCustomerInTable->next;
			}
			while (line_1_backup != NULL){
				customer* DeleteCustomer = line_1_backup;
				line_1_backup = line_1_backup->prev;
				if (line_1_backup != NULL){
					line_1_backup->next = NULL;
				}
				DeleteCustomer->prev = NULL;
				delete DeleteCustomer;
			}
			while (line_2_backup != NULL){
				customer* DeleteCustomer = line_2_backup;
				line_2_backup = line_2_backup->prev;
				if (line_2_backup != NULL){
					line_2_backup->next = NULL;
				}
				DeleteCustomer->prev = NULL;
				delete DeleteCustomer;
			}
		}
		
		void UNLIMITED_VOID()
		{
			if (CustomerInTable < 4){ // Neu nhu ban dau khach torng ban < 4 -> khong co day con co so phan tu lien tiep >= 4
				return; ///////////////////////////// -> khong lam gi ca
			}
			else {
				customer* res = CurrentCustomerInTable;
				int sum = 1e9 + 7; // Tong nho nhat
				int max_len = 0; // Chieu dai dai nhat
				
				customer* tmp = CurrentCustomerInTable;
				for (int i = 1; i <= CustomerInTable; i++){
					customer* temp = tmp;
					int tong = 0;
					int len = 0;
					for (int j = 1; j <= CustomerInTable; j++){
						tong += temp->energy;
						len++;
						if (len >= 4){
							if (tong < sum || (tong == sum && max_len <= len)){
								sum = tong;
								max_len = len;
								res = tmp;
							}
						}
						temp = temp->next;
					}
					tmp = tmp->next;
				}
				customer* run = res;
				customer* st = res; // Bien de tim phan tu nho nhat trong day con tim duoc
				int cnt = 1;
				for (int i = 1; i <= max_len; i++){
					if (st->energy > run->energy){
						st = run;
						cnt = i; // Xac dinh vi tri bat dau de in cus
					}
					run = run->next;
				}
				for (int i = cnt; i <= max_len; i++){ // In cus tu cus_min den het da con theo chieu KDH
					st->print();
					st = st->next;
				}
				for (int i = 1; i < cnt; i++){ // In ra so phan tu con lai theo Chieu KDH
					res->print();
					res = res->next;
				}
			}
		}
		
		void DOMAIN_EXPANSION()
		{
			if (ChuThuatSu == 0 || OanLinh == 0) {
				return;
			}
			if (ChuThuatSu >= abs(OanLinh)){// Xoa toan bo Oan linh
				OanLinh = 0;
				customer* tmp = OrderTail;
				while (tmp != nullptr){
					if (tmp->energy < 0){
						tmp->print();
					}
					tmp = tmp->prev;
				}
				tmp = OrderHead;
				while (tmp != nullptr){
					if (tmp->energy < 0){
						customer* DeleteCustomer = tmp;
						if (tmp->name == OrderHead->name){
							OrderHead = OrderHead->next;
							if (OrderHead != nullptr){
								OrderHead->prev = nullptr;
							}
							else OrderTail = nullptr;
						}
						else if (tmp->name == OrderTail->name){
							OrderTail = OrderTail->prev;
							if (OrderTail != nullptr){
								OrderTail->next = nullptr;
							}
							else OrderHead = nullptr;
						}
						else {
							DeleteCustomer->prev->next = DeleteCustomer->next;
							DeleteCustomer->next->prev = DeleteCustomer->prev;
						}
						customer* khach_trong_ban = KIEMTRAKHACHTRONGBAN(tmp->name);
						if (khach_trong_ban != nullptr){
							khach_trong_ban->prev->next = khach_trong_ban->next;
							khach_trong_ban->next->prev = khach_trong_ban->prev;
							CurrentCustomerInTable = khach_trong_ban->prev;
							khach_trong_ban->prev = nullptr;
							khach_trong_ban->next = nullptr;
							delete khach_trong_ban;
							CustomerInTable -= 1;
						}
						else {
							customer* khach_trong_hang_doi = KIEMTRAKHACHTRONGHANGDOI(tmp->name);
							if (khach_trong_hang_doi->name == QueueHead->name){
								QueueHead = QueueHead->next;
								if (QueueHead != nullptr){
									QueueHead->prev = nullptr;
								}
								else QueueTail = nullptr;
							}
							else if (khach_trong_hang_doi->name == QueueTail->name){
								QueueTail = QueueTail->prev;
								if (QueueTail != nullptr){
									QueueTail->next = nullptr;
								}
								else QueueHead = nullptr;
							}
							else {
								khach_trong_hang_doi->prev->next = khach_trong_hang_doi->next;
								khach_trong_hang_doi->next->prev = khach_trong_hang_doi->prev;
							}
							khach_trong_hang_doi->prev = nullptr;
							khach_trong_hang_doi->next = nullptr;
							delete khach_trong_hang_doi;
							CustomerInQueue -= 1;
						}
						tmp = tmp->next;
						DeleteCustomer->next = nullptr;
						DeleteCustomer->prev = nullptr;
						delete DeleteCustomer;

					}
					else tmp = tmp->next;
				}
			}
			else {
				ChuThuatSu = 0;
				customer* tmp = OrderTail;
				while (tmp != nullptr){
					if (tmp->energy > 0){
						tmp->print();
					}
					tmp = tmp->prev;
				}
				tmp = OrderHead;
				while (tmp != nullptr){
					if (tmp->energy > 0){
						customer* DeleteCustomer = tmp;
						if (tmp->name == OrderHead->name){
							OrderHead = OrderHead->next;
							if (OrderHead != nullptr){
								OrderHead->prev = nullptr;
							}
							else OrderTail = nullptr;
						}
						else if (tmp->name == OrderTail->name){
							OrderTail = OrderTail->prev;
							if (OrderTail != nullptr){
								OrderTail->next = nullptr;
							}
							else OrderHead = nullptr;
						}
						else {
							DeleteCustomer->prev->next = DeleteCustomer->next;
							DeleteCustomer->next->prev = DeleteCustomer->prev;
						}
						
						customer* khach_trong_ban = KIEMTRAKHACHTRONGBAN(tmp->name);
						if (khach_trong_ban != nullptr){
							khach_trong_ban->prev->next = khach_trong_ban->next;
							khach_trong_ban->next->prev = khach_trong_ban->prev;
							CurrentCustomerInTable = khach_trong_ban->next;
							khach_trong_ban->prev = nullptr;
							khach_trong_ban->next = nullptr;
							delete khach_trong_ban;
							CustomerInTable -= 1;
						}
						else {
							customer* khach_trong_hang_doi = KIEMTRAKHACHTRONGHANGDOI(tmp->name);
							if (khach_trong_hang_doi->name == QueueHead->name){
								QueueHead = QueueHead->next;
								if (QueueHead != nullptr){
									QueueHead->prev = nullptr;
								}
								else QueueTail = nullptr;
							}
							else if (khach_trong_hang_doi->name == QueueTail->name){
								QueueTail = QueueTail->prev;
								if (QueueTail != nullptr){
									QueueTail->next = nullptr;
								}
								else QueueHead = nullptr;
							}
							else {
								khach_trong_hang_doi->prev->next = khach_trong_hang_doi->next;
								khach_trong_hang_doi->next->prev = khach_trong_hang_doi->prev;
							}
							khach_trong_hang_doi->prev = nullptr;
							khach_trong_hang_doi->next = nullptr;
							delete khach_trong_hang_doi;
							CustomerInQueue -= 1;
						}
						
						tmp = tmp->next;
						DeleteCustomer->next = nullptr;
						DeleteCustomer->prev = nullptr;
						delete DeleteCustomer;

					}
					else tmp = tmp->next; 
				}
			}
			DICHUYEN();
		}

		void LIGHT(int num)
		{
			
			if (num != 0){
				customer* tmp = CurrentCustomerInTable;
				if (num > 0){
					for (int i = 1; i <= CustomerInTable; i++){
						tmp->print();
						tmp = tmp->next;
					}
				}
				else {
					num = -num;
					for (int i = 1; i <= CustomerInTable; i++){
						tmp->print();
						tmp = tmp->prev;
					}
				}
			}
			else {
				customer* tmp = QueueHead;
				while (tmp != nullptr){
					tmp->print();
					tmp = tmp->next;
				}
			}
		}
};
