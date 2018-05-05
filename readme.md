# Thông tin cá nhân
- MSSV: 1512557
- Họ tên: Phan Trọng Thuyên
- Email: phantrongthuyen9d@gmail.com
# Main function
1. Người dùng nhấn tổ hợp phím Ctrl + Space, màn hình sẽ xuất hiện một cửa sổ gồm 2 control, 1 textbox để nhập
dữ liệu và 1 listview để hiển thị dữ liệu truy cập.
2. Mỗi khi người dùng nhấn vào một chữ cái, các tập tin có đuôi .exe sẽ hiện ra tương ứng với chữ cái đó (nếu có)
3. Người dùng có thể sử dụng phím mũi tên lên, xuống để lựa chọn các file trong listview
4. Khi người dùng nhấn Enter hoặc double click thì chương trình được chọn sẽ khởi chạy.
5. Người dùng có thể xoá các item trên listview
6. Có biểu đồ phân tích những ứng dụng sử dụng thường xuyên
## Main success

1. Người dùng chạy chương trình Quick Launch, một icon được thêm vào ở vùng notification, một thông báo hiển thị 
cho người dùng là nhấn Ctrl + Space để mở giao diện của chương trình lên.
2. Người dùng có thể click chuột phải vào icon ở notification area, một menu lựa chọn sẽ hiển thị lên cho phép người dùng chọn những chức năng sau:
- Scan to build database, người dùng có thể lựa chọn folder để load hoặc chọn load mặc định từ Program Files và Program Files (x86). Dữ liệu được lưu vào cơ sở dữ liệu là vector<Program> với cấu trúc dữ liệu Program gồm các thông tin thuộc tính của file.
- View statistics: Chương trình đưa ra thống kê bằng biểu đồ tròn, các ứng dụng thường sử dụng (6 ứng dụng) sẽ được mô tả trên biểu đồ, cùng phần trăm thể hiện mức độ truy cập.
- Exit: Thoát hoàn toàn chương trình
3. Khi chạy chương trình, người dùng nhấn Ctr+Space để hiển thị, nhấn một lần nữa để ẩn
4. Người dùng có thể lựa chọn chương trình bằng phím mũi tên lên xuống trong list view
5. Người dùng ấn enter vào item đã chọn, chương trình tương ứng sẽ khởi động

## Additional flows
1. Chương trình chỉ thoát khi người dùng nhấn Exit trong menu, nhấn X chương trình sẽ ẩn
2. Bắt đầu chương trình sẽ có thông báo nhắc
3. Khi khởi động, chương trình sẽ nạp một số dữ liệu cơ sở từ 3 lớp folder của Program Files và Program Files (x86)
4. Sử dụng load icon, load các icon của chương trình lựa chọn tương ứng
5. Hiển thị thông tin như tên, đường dẫn đến tệp tin

## Các luồng sự kiện chính
> Chạy chương trình, nạp dữ liệu từ file nếu có
> Hiển thị textbox, listview, một số chương trình trong cơ sở dữ liệu
> Nhập chữ vào textbox, listview sẽ đưa ra chương trình tương ứng với chữ cái
> Thông kê biểu đồ được hiển thị ở một dialog, có các chú thích tương ứng
> Người dùng thoát chương trình lưu lại dữ liệu xuống file

## Hướng dẫn sử dụng
> Người dùng mở chương trình, nhấn Ctrl+Space để vào giao diện
> Người dùng nhấn chuột phải vào icon ở notification area, menu sẽ xuất hiện
> Nhập chữ vào ô tìm kiếm
> Chọn chương trình tương ứng
> Double click hoặc enter để khởi chạy
> Thoát chương trình, dữ liệu sẽ tự động lưu xuống.

## Yêu cầu khác
> Link repo: https://billhcmus@bitbucket.org/billhcmus/personal-finance.git

> Link video: https://youtu.be/vrbAdTRoOTY

> Nền tảng build: Visual Studio 2017
