#向表中插入数据
#不要用中文！！！否则c++ mysql库无法正常读取/查询（乱码），记得改
insert into FLIGHTS values
('A-2215456', 600, 30, 30, "南京", "北京"),
('A-2211457', 1000, 50, 50, "广州", "山西"),
('A-2211706', 600, 30, 30, "西安", "上海"),
('D-2113941', 1500, 35, 35, "上海", "西安"),
('A-4312940', 1300, 40, 40, "北京", "上海"),
('T-1118480', 1000, 35, 35, "山西", "广州"),
('A-5410349', 800, 30, 30, "南京", "广州"),
('D-3210054', 700, 35, 35, "山西", "南京"),
('G-2717790', 1200, 40, 40, "北京", "武汉"),
('D-8015010', 800, 35, 35, "西安", "天津"),
('A-1111480', 680, 30, 30, "广州", "北京"),
('A-2215723', 1299, 50, 50, "上海", "南京"),
('T-4312643', 500, 30, 30, "广州", "西安"),
('A-5618012', 1300, 30, 30, "山西", "上海"),
('A-1011254', 800, 50, 50, "北京", "西安"),
('T-1014566', 700, 55, 55, "北京", "南京"),
('A-2714324', 700, 45, 45, "山西", "北京"),
('G-3114878', 1000, 50, 50, "广州", "上海"),
('G-4218785', 1100, 55, 55, "西安", "广州");

insert into HOTELS values
('H-184', "山西", 298, 60, 60),
('H-632', "山西", 298, 60, 60),
('H-40', "山西", 298, 60, 60),
('H-2379', "山西", 298, 60, 60),
('H-609', "山西", 298, 60, 60),
('H-226', "西安", 400, 70, 70),
('H-274', "西安", 400, 70, 70),
('H-253', "西安", 400, 70, 70),
('H-227', "西安", 400, 70, 70),
('H-291', "北京", 800, 95, 95),
('H-979', "北京", 800, 95, 95),
('H-200', "北京", 800, 95, 95),
('H-222', "北京", 800, 95, 95),
('H-246', "北京", 800, 95, 95),
('H-221', "北京", 800, 95, 95),
('H-295', "上海", 999, 60, 60),
('H-385', "上海",999, 60, 60),
('H-99', "上海", 999, 60, 60),
('H-296', "广州", 799, 80, 80),
('H-201', "广州", 799, 80, 80),
('H-322', "广州", 799, 80, 80);

insert into BUS values
('B-395', "山西", 60, 50, 50),
('B-844', "山西", 60, 50, 50),
('B-181', "山西", 60, 50, 50),
('B-184', "山西", 60, 50, 50),
('B-849', "西安", 80, 65, 65),
('B-130', "西安", 80, 65, 65),
('B-461', "西安", 80, 65, 65),
('B-321', "北京", 200, 45, 45),
('B-609', "北京", 200, 45, 45),
('B-679', "上海", 700, 55, 55),
('B-509', "上海", 700, 55, 55),
('B-909', "上海", 700, 55, 55),
('B-365', "广州", 500, 65, 65),
('B-884', "广州", 500, 65, 65),
('B-800', "广州", 500, 65, 65);