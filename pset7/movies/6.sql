SELECT
	AVG(r.rating)
FROM
	ratings AS r
JOIN
	movies AS m
		ON m.id = r.movie_id
WHERE
	m.year = 2012;
	