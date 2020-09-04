SELECT
	COUNT(*)
FROM
	movies AS m
JOIN
	ratings AS r
		ON m.id = r.movie_id
WHERE
	r.rating = 10.0;