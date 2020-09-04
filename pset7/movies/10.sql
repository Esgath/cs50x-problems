SELECT
	p.name
FROM
	people AS p
JOIN
	directors AS d
		ON p.id = d.person_id
JOIN
	ratings AS r
		ON r.movie_id = d.movie_id
WHERE
	r.rating >= 9.0
GROUP BY
	p.name;