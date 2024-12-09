#很基础的触发器设置，暂时没想到其他约束
#预订后更新flights/hotels/bus中的信息
DELIMITER $$

CREATE TRIGGER data_update
AFTER INSERT ON RESERVATIONS
FOR EACH ROW
BEGIN
    IF NEW.resvType = 1 THEN
        UPDATE FLIGHTS
        SET numAvail = numAvail - 1
        WHERE flightNum = NEW.resvKey;
    ELSEIF NEW.resvType = 2 THEN
        UPDATE HOTELS
        SET numAvail = numAvail - 1
        WHERE hotelNum = NEW.resvKey;
    ELSEIF NEW.resvType = 3 THEN
        UPDATE BUS
        SET numAvail = numAvail - 1
        WHERE busNum = NEW.resvKey;
    END IF;
END$$

DELIMITER ;

#取消预订后更新表中的信息
DELIMITER $$

CREATE TRIGGER data_update_del 
AFTER DELETE ON RESERVATIONS
FOR EACH ROW
BEGIN
	IF OLD.resvType = 1 THEN
		UPDATE	FLIGHTS
		SET		numAvail = numAvail + 1
		WHERE	flightNum = old.resvKey;
	ELSEIF OLD.resvType = 2 THEN
		UPDATE	HOTELS
		SET		numAvail = numAvail + 1
		WHERE	hotelNum = old.resvKey;
	ELSEIF OLD.resvType = 3 THEN
		UPDATE	BUS
		SET		numAvail = numAvail + 1
		WHERE	busNum = old.resvKey;
	END IF;
END$$

DELIMITER ;

show TRIGGERS;